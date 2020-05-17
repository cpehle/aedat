#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "aedat.hpp"

namespace dvs_gesture {
struct DataSet {
  struct Row {
    uint32_t label;
    uint32_t startTime;
    uint32_t endTime;
  };

  struct DataPoint {
    uint32_t label;
    std::vector<AEDAT::PolarityEvent> events;
  };

  void load(std::string filename) {
    std::string labels_filename = filename + "_labels.csv";
    std::string aedat_filename = filename + ".aedat";

    std::fstream fs;
    char line[256];

    fs.open(labels_filename, std::fstream::in);
    fs.getline(line, 256);
    std::vector<Row> rows;

    while (!fs.eof()) {
      Row row;
      fs >> row.label;
      fs.ignore(1);
      fs >> row.startTime;
      fs.ignore(1);
      fs >> row.endTime;
      fs.ignore(1);
      rows.push_back(row);
    }
    rows.pop_back();

    AEDAT data;
    data.load(aedat_filename);

    size_t event_idx = 0;
    for (auto row : rows) {

      while (data.polarity_events[event_idx].timestamp < row.startTime) {
        event_idx++;
      }

      DataPoint p{row.label};
      while (data.polarity_events[event_idx].timestamp < row.endTime) {
        p.events.push_back(data.polarity_events[event_idx]);
        event_idx++;
      }

      datapoints.push_back(p);
    }
  }

  std::vector<DataPoint> datapoints;
};
} // namespace dvs_gesture
#include "spec.hpp"

string cmd = "";
bool is_high = false;

int main() {
  assert_commands();
  read_binary();
  // read_binary("tmp.mdl");
  // read_binary("mdl/FD120_src.mdl");

  VI curpos = {0, max_y + 1, 0};
  for (int y = max_y; y >= 0; y--) {
    int min_x = 1000, min_z = 1000;
    int max_x = -1, max_z = -1;
    for (int i = 0; i < R; i++) {
      for (int j = 0; j < R; j++) {
        if (model[i][y][j]) {
          min_x = min(min_x, i);
          min_z = min(min_z, j);
          max_x = max(max_x, i);
          max_z = max(max_z, j);
        }
      }
    }

    VI startpos = {min_x, y + 1, min_z};
    int xoff = 0;
    int zoff = 0;
    int xoff_max = max_x - min_x;
    int zoff_max = max_z - min_z;
    while (zoff <= zoff_max) {
      if (model[startpos[0] + xoff][y][startpos[2] + zoff]) {
        query.push_back(id(startpos[0] + xoff, y, startpos[2] + zoff));
      }
      if (zoff % 2 == 0) {
        if (xoff < xoff_max) {
          xoff++;
        } else {
          zoff++;
        }
      } else {
        if (xoff > 0) {
          xoff--;
        } else {
          zoff++;
        }
      }
    }
    if (y == 0) {
      break;
    }
    curpos = {startpos[0] + xoff, y, startpos[2] + zoff};
  }

  solve_query();

  cmd += move_from_to({0, 0, 0}, {0, max_y + 1, 0});
  curpos = {0, max_y + 1, 0};
  int ind = 0;
  for (int y = max_y; y >= 0; y--) {
    int min_x = 1000, min_z = 1000;
    int max_x = -1, max_z = -1;
    for (int i = 0; i < R; i++) {
      for (int j = 0; j < R; j++) {
        if (model[i][y][j]) {
          min_x = min(min_x, i);
          min_z = min(min_z, j);
          max_x = max(max_x, i);
          max_z = max(max_z, j);
        }
      }
    }
    cmd += move_from_to(curpos, {min_x, y + 1, min_z});
    curpos = {min_x, y + 1, min_z};
    VI startpos = {min_x, y + 1, min_z};
    int xoff = 0;
    int zoff = 0;
    int xoff_max = max_x - min_x;
    int zoff_max = max_z - min_z;
    while (zoff <= zoff_max) {
      if (model[startpos[0] + xoff][y][startpos[2] + zoff]) {
        cmd += long_move_s(curpos[0], startpos[0] + xoff, x_axis);
        cmd += long_move_s(curpos[2], startpos[2] + zoff, z_axis);
        cmd += long_move_s(curpos[1], y + 1, y_axis);
        curpos = {startpos[0] + xoff, y + 1, startpos[2] + zoff};
        if (!is_high && query_ans[ind]) {
          cmd += void_s({0, -1, 0});
        }
        if (!is_high && !query_ans[ind]) {
          cmd += flip_s();
          cmd += void_s({0, -1, 0});
          is_high = true;
        }
        if (is_high && query_ans[ind]) {
          cmd += void_s({0, -1, 0});
          cmd += flip_s();
          is_high = false;
        }
        if (is_high && !query_ans[ind]) {
          cmd += void_s({0, -1, 0});
        }
        ind++;
      }
      if (zoff % 2 == 0) {
        if (xoff < xoff_max) {
          xoff++;
        } else {
          zoff++;
        }
      } else {
        if (xoff > 0) {
          xoff--;
        } else {
          zoff++;
        }
      }
    }
    if (y == 0) {
      cmd += move_from_to(curpos, {0, 0, 0});
      curpos = {0, 0, 0};
      break;
    }
  }
  cmd += halt_s();

  write_binary(cmd);
  // write_binary("tmp.nbt", cmd);
}

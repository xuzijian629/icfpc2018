#include "spec.hpp"

string cmd = "";
bool is_high = false;

int main() {
  assert_commands();
  read_binary("tmp.mdl");

  cmd += move_from_to({0, 0, 0}, {0, max_y + 1, 0});
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
    cmd += move_from_to(curpos, {min_x, y + 1, min_z});
    VI startpos = {min_x, y + 1, min_z};
    int xoff = 0;
    int zoff = 0;
    int xoff_max = max_x - min_x;
    int zoff_max = max_z - min_z;
    while (zoff <= zoff_max) {
      if (model[startpos[0] + xoff][y][startpos[2] + zoff]) {
        model[startpos[0] + xoff][y][startpos[2] + zoff] = 0;
        cnt--;
        if (!is_high && can_void(startpos[0] + xoff, y, startpos[2] + zoff)) {
          cmd += void_s({0, -1, 0});
        }
        if (!is_high && !can_void(startpos[0] + xoff, y, startpos[2] + zoff)) {
          cmd += flip_s();
          cmd += void_s({0, -1, 0});
          is_high = true;
        }
        if (is_high && can_void(startpos[0] + xoff, y, startpos[2] + zoff)) {
          cmd += void_s({0, -1, 0});
          cmd += flip_s();
          is_high = false;
        }
        if (is_high && !can_void(startpos[0] + xoff, y, startpos[2] + zoff)) {
          cmd += void_s({0, -1, 0});
        }
      }
      if (zoff % 2 == 0) {
        if (xoff < xoff_max) {
          xoff++;
          cmd += smove_s({1, 0, 0});
        } else {
          zoff++;
          cmd += smove_s({0, 0, 1});
        }
      } else {
        if (xoff > 0) {
          xoff--;
          cmd += smove_s({-1, 0, 0});
        } else {
          zoff++;
          cmd += smove_s({0, 0, 1});
        }
      }
    }
    if (y == 0) {
      cmd += move_from_to({startpos[0] + xoff, y + 1, startpos[2] + zoff}, {0, 0, 0});
      break;
    }
    cmd += smove_s({0, -1, 0});
    curpos = {startpos[0] + xoff, y, startpos[2] + zoff};
  }
  cmd += halt_s();

  write_binary("tmp.nbt", cmd);
}

#include "spec.hpp"

enum Axis { x_axis, y_axis, z_axis };
struct voxel { int x, y, z; };
voxel p = {0, 0, 0};
stringstream inst;

void long_move_s(int from, int to, Axis ax) {
  while (from != to) {
    int dist = max(-15, min(15, to - from));
    switch(ax) {
      case x_axis: inst << smove_s({dist, 0, 0}); break;
      case y_axis: inst << smove_s({0, dist, 0}); break;
      case z_axis: inst << smove_s({0, 0, dist}); break;
    }
    from += dist;
  }
}

void remove_tails() {
}

int go_highest() {
  for (int y = R-1; y >= 0; y--) {
    for (int z = 0; z < R; z++) {
      for (int x = 0; x < R; x++) {
        if (model[x][y][z]) {
          long_move_s(0, y, y_axis);
          long_move_s(0, z, z_axis);
          long_move_s(0, x-1, x_axis);
          p = {x-1, y, z};
          inst << void_s({1, 0, 0});
          model[x][y][z] = 0;
          return y;
        }
      }
    }
  }
  assert (1);
  return -10000;
}

void deconstruct() {
  remove_tails();
  int y_max = go_highest();
  for (int y = y_max; y >= 0; y--) {
    for (int z = 0; z < R; z++) {
      for (int x = 0; x < R; x++) {
        if (model[x][y][z]) {
          long_move_s(p.x, x-1, x_axis);
          long_move_s(p.z, z, z_axis);
          p = {x-1, y, z};
          inst << void_s({1, 0, 0});
          model[x][y][z] = 0;
        }
      }
    }
    long_move_s(p.x, 0, x_axis);
    long_move_s(p.z, 0, z_axis);
    if (y > 0) {
      inst << smove_s({0, -1, 0});
      p = {0, y-1, 0};
    }
  }
  inst << halt_s();
  write_binary(inst.str());
}

int main() {
  assert_commands();
  read_binary();
  deconstruct();
}

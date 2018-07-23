#include "spec.hpp"

int main() {
  read_binary("mdl/FA001_tgt.mdl");
  VI pillar = get_pillar();

  cmd += move_from_to(0, pillar.front());

  for (int i = 1; i < min(int(pillar.size()), 39); i++) {
    cmd += fission_s({1, 0, 0}, i);
    for (int j = 0; j < i - 1; j++) {
      cmd += wait_s();
    }
    cmd += move_from_to(pillar[i - 1], pillar[i]);
    for (int j = 0; j < i; j++) {
      if (j == i - 1) {
        cmd += fill_s({-1, 0, 0});
      } else {
        cmd += wait_s();
      }
    }
  }
  node_cmd[1].push(smove_s({0, 1, 0}));
  nodes = min(int(pillar.size()), 39);
  for (int i = 2; i <= nodes; i++) {
    vector<string> node_sol = solve2D(i);
    for (string c: node_sol) {
      node_cmd[i].push(c);
    }
  }

  while (!emptyAll()) {
    cmd += gather_cmd();
  }

  // 適切に停止する

  write_binary("tmp.nbt", cmd);
}

#include <bits/stdc++.h>
using namespace std;
using VI = vector<int>;
using VVI = vector<VI>;
using VVVI = vector<VVI>;

int R;
VVVI model;
int nodes;
queue<string> node_cmd[40];
int nodepos[40];
string cmd = "";

void print2D(int y) {
  cout << "y: " << y << endl;
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < R; j++) {
      if (model[i][y][j]) {
        cout << '#';
      } else {
        cout << '.';
      }
    }
    cout << endl;
  }
  cout << endl;
}

int id(int x, int y, int z) {
  return x * R * R + y * R + z;
}

VI xyz(int id) {
  int x = id / (R * R);
  int y = (id % (R * R)) / R;
  int z = id % R;
  return {x, y, z};
}

VI diffs[6] = {{0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}};

VI get_pillar() {
  using P = pair<int, int>;
  using VP = vector<P>;
  using VVP = vector<VP>;
  using VVVP = vector<VVP>;
  VVVP memo = VVVP(R, VVP(R, VP(R, P(1000, -1))));
  queue<int> q;
  for (int x = 0; x < R; x++) {
    for (int z = 0; z < R; z++) {
      if (model[x][0][z]) {
        memo[x][0][z] = P(1, -1);
        q.push(id(x, 0, z));
      }
    }
  }
  while (!q.empty()) {
    int i = q.front();
    VI c = xyz(i);
    q.pop();
    int from_v = memo[c[0]][c[1]][c[2]].first;
    for (VI d: diffs) {
      if (0 < c[0] + d[0] && c[0] + d[0] < R && 0 <= c[1] + d[1] && c[1] + d[1] < R && 0 < c[2] + d[2] && c[2] + d[2] < R) {
        int to_v = memo[c[0] + d[0]][c[1] + d[1]][c[2] + d[2]].first;
        if (model[c[0] + d[0]][c[1] + d[1]][c[2] + d[2]] && to_v > from_v) {
          memo[c[0] + d[0]][c[1] + d[1]][c[2] + d[2]] = P(from_v + 1, i);
          q.push(id(c[0] + d[0], c[1] + d[1], c[2] + d[2]));
        }
      } else {
        continue;
      }
    }
  }

  int y, dist, x, z;
  for (y = R - 1; y >= 0; y--) {
    dist = 1000, x = 1000, z = 1000;
    for (int i = 0; i < R; i++) {
      for (int j = 0; j < R; j++) {
        if (memo[i][y][j].first != 1000 && memo[i][y][j].first <= dist) {
          if (memo[i][y][j].first < dist) {
            x = i;
            z = j;
          } else {
            if (i + j < x + z) {
              x = i;
              z = j;
            }
          }
          dist = memo[i][y][j].first;
        }
      }
    }
    if (dist != 1000) {
      break;
    }
  }

  VI ret;
  while (1) {
    assert(model[x][y][z]);
    ret.push_back(id(x, y, z));
    if (memo[x][y][z].second == -1) {
      break;
    }
    int prev = memo[x][y][z].second;
    VI c = xyz(prev);
    x = c[0];
    y = c[1];
    z = c[2];
  }

  reverse(ret.begin(), ret.end());
  return ret;
}

string halt_s() {
  return "11111111";
}

string wait_s() {
  return "11111110";
}

string flip_s() {
  return "11111101";
}

string smove_s(VI v) {
  assert(v.size() == 3 && (v[0] != 0) + (v[1] != 0) + (v[2] != 0) == 1);
  string dir;
  if (v[0]) dir += "01";
  if (v[1]) dir += "10";
  if (v[2]) dir += "11";
  int dist = v[0] + v[1] + v[2];
  assert(abs(dist) <= 15);
  dist += 15;
  string len = "";
  len += '0' + ((dist >> 4) & 1);
  len += '0' + ((dist >> 3) & 1);
  len += '0' + ((dist >> 2) & 1);
  len += '0' + ((dist >> 1) & 1);
  len += '0' + (dist & 1);
  string ret = "00";
  ret += dir + "0100000" + len;
  return ret;
}

string lmove_s(VI v1, VI v2) {
  assert(v1.size() == 3 && (v1[0] != 0) + (v1[1] != 0) + (v1[2] != 0) == 1);
  assert(v2.size() == 3 && (v2[0] != 0) + (v2[1] != 0) + (v2[2] != 0) == 1);
  string dir1, dir2;
  if (v1[0]) dir1 += "01";
  if (v1[1]) dir1 += "10";
  if (v1[2]) dir1 += "11";
  if (v2[0]) dir2 += "01";
  if (v2[1]) dir2 += "10";
  if (v2[2]) dir2 += "11";
  int dist1 = v1[0] + v1[1] + v1[2];
  assert(abs(dist1) <= 5);
  dist1 += 5;
  int dist2 = v2[0] + v2[1] + v2[2];
  assert(abs(dist2) <= 5);
  dist2 += 5;
  string len1 = "", len2 = "";
  len1 += '0' + ((dist1 >> 3) & 1);
  len1 += '0' + ((dist1 >> 2) & 1);
  len1 += '0' + ((dist1 >> 1) & 1);
  len1 += '0' + (dist1 & 1);
  len2 += '0' + ((dist2 >> 3) & 1);
  len2 += '0' + ((dist2 >> 2) & 1);
  len2 += '0' + ((dist2 >> 1) & 1);
  len2 += '0' + (dist2 & 1);
  string ret = dir2 + dir1 + "1100" + len2 + len1;
  return ret;
}

string move_from_to(int fid, int tid) {
  // assume there is no obstacle
  if (fid == tid) return "";
  VI f = xyz(fid);
  VI t = xyz(tid);
  int dx, dy, dz;
  dx = t[0] - f[0];
  dy = t[1] - f[1];
  dz = t[2] - f[2];
  if ((dx == 0 && dy == 0) && (dy == 0 && dz == 0) && (dz == 0 && dx == 0)) {
    return smove_s({dx, dy, dz});
  }
  // うち1つは0だと仮定
  assert(dx == 0 || dy == 0 || dz == 0);
  int fdx = 0, fdy = 0, fdz = 0;
  if (dx > 5) fdx = dx - 5;
  if (dy > 5) fdy = dy - 5;
  if (dz > 5) fdy = dz - 5;
  if (dx < -5) fdx = dx + 5;
  if (dy < -5) fdy = dy + 5;
  if (dz < -5) fdy = dz + 5;
  return "";
}

string fusionp_s(VI v) {
  int dist = (v[0] + 1) * 9 + (v[1] + 1) * 3 + (v[2] + 1);
  string len = "";
  len += '0' + ((dist >> 4) & 1);
  len += '0' + ((dist >> 3) & 1);
  len += '0' + ((dist >> 2) & 1);
  len += '0' + ((dist >> 1) & 1);
  len += '0' + (dist & 1);
  return len + "111";
}

string fusions_s(VI v) {
  int dist = (v[0] + 1) * 9 + (v[1] + 1) * 3 + (v[2] + 1);
  string len = "";
  len += '0' + ((dist >> 4) & 1);
  len += '0' + ((dist >> 3) & 1);
  len += '0' + ((dist >> 2) & 1);
  len += '0' + ((dist >> 1) & 1);
  len += '0' + (dist & 1);
  return len + "110";
}

string fission_s(VI v, int m) {
  int dist = (v[0] + 1) * 9 + (v[1] + 1) * 3 + (v[2] + 1);
  string len = "", ms;
  len += '0' + ((dist >> 4) & 1);
  len += '0' + ((dist >> 3) & 1);
  len += '0' + ((dist >> 2) & 1);
  len += '0' + ((dist >> 1) & 1);
  len += '0' + (dist & 1);
  ms += '0' + ((m >> 7) & 1);
  ms += '0' + ((m >> 6) & 1);
  ms += '0' + ((m >> 5) & 1);
  ms += '0' + ((m >> 4) & 1);
  ms += '0' + ((m >> 3) & 1);
  ms += '0' + ((m >> 2) & 1);
  ms += '0' + ((m >> 1) & 1);
  ms += '0' + (m & 1);
  return len + "101" + ms;
}

string fill_s(VI v) {
  int dist = (v[0] + 1) * 9 + (v[1] + 1) * 3 + (v[2] + 1);
  string len = "";
  len += '0' + ((dist >> 4) & 1);
  len += '0' + ((dist >> 3) & 1);
  len += '0' + ((dist >> 2) & 1);
  len += '0' + ((dist >> 1) & 1);
  len += '0' + (dist & 1);
  return len + "011";
}

string void_s(VI v) {
  int dist = (v[0] + 1) * 9 + (v[1] + 1) * 3 + (v[2] + 1);
  string len = "";
  len += '0' + ((dist >> 4) & 1);
  len += '0' + ((dist >> 3) & 1);
  len += '0' + ((dist >> 2) & 1);
  len += '0' + ((dist >> 1) & 1);
  len += '0' + (dist & 1);
  return len + "010";
}

void assert_commands() {
  assert(smove_s({12,0,0}) == "0001010000011011");
  assert(smove_s({0,0,-4}) == "0011010000001011");
  assert(lmove_s({3,0,0},{0,-5,0}) == "1001110000001000");
  assert(lmove_s({0,-2,0},{0,0,2}) == "1110110001110011");
  assert(fusionp_s({-1,1,0}) == "00111111");
  assert(fusions_s({1,-1,0}) == "10011110");
  assert(fission_s({0,0,1}, 5) == "0111010100000101");
  assert(fill_s({0,-1,0}) == "01010011");
  assert(void_s({1,0,1}) == "10111010");
}

void read_binary(string s) {
  FILE *fp;
  if ((fp = fopen(s.c_str(), "rb")) == NULL) {
    cerr << "Failed to open file" << endl;
    exit(1);
  }

  uint8_t r;
  fread(&r, sizeof(uint8_t), 1, fp);
  R = int(r);
  model = VVVI(R, VVI(R, VI(R)));
  string bin = "";
  uint8_t c[2000000];
  assert(ceil(R * R * R / 8.0) < 2000000);
  fread(c, sizeof(uint8_t), int(ceil(R * R * R / 8.0)), fp);
  fclose(fp);
  for (int i = 0; i < int(ceil(R * R * R / 8.0)); i++) {
    bin += '0' + (c[i] & 1);
    bin += '0' + ((c[i] >> 1) & 1);
    bin += '0' + ((c[i] >> 2) & 1);
    bin += '0' + ((c[i] >> 3) & 1);
    bin += '0' + ((c[i] >> 4) & 1);
    bin += '0' + ((c[i] >> 5) & 1);
    bin += '0' + ((c[i] >> 6) & 1);
    bin += '0' + ((c[i] >> 7) & 1);
  }
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < R; j++) {
      for (int k = 0; k < R; k++) {
        model[i][j][k] = bin[i * R * R + j * R + k] == '1';
      }
    }
  }
  cout << "Model successfully imported" << endl;
}

void read_binary() {
  cout << "Model file name?" << endl;
  string s;
  cin >> s;
  read_binary(s);
}

void write_binary(string s, string cmd) {
  FILE *fp;
  if ((fp = fopen(s.c_str(), "wb")) == NULL) {
    cerr << "Failed to open file" << endl;
    exit(1);
  }
  assert(cmd.size() % 8 == 0);
  uint8_t c[2000000];
  assert(cmd.size() / 8 < 2000000);
  for (int i = 0; i < cmd.size(); i+=8) {
    uint8_t e = 0;
    e += 128 * (cmd[i] == '1');
    e += 64 * (cmd[i + 1] == '1');
    e += 32 * (cmd[i + 2] == '1');
    e += 16 * (cmd[i + 3] == '1');
    e += 8 * (cmd[i + 4] == '1');
    e += 4 * (cmd[i + 5] == '1');
    e += 2 * (cmd[i + 6] == '1');
    e += cmd[i + 7] == '1';
    c[i / 8] = e;
  }
  fwrite(c, sizeof(uint8_t), cmd.size() / 8, fp);
  fclose(fp);
}

void write_binary(string cmd) {
  cout << "Trace file name?" << endl;
  string s;
  cin >> s;
  write_binary(s, cmd);
}

vector<string> solve2D(int node) {
  return {""};
}

bool emptyAll() {
  for (int i = 0; i < 40; i++) {
    if (!node_cmd[i].empty()) return false;
  }
  return true;
}

string gather_cmd() {
  string ret = "";
  for (int i = 1; i <= nodes; i++) {
    if (!node_cmd[i].empty()) {
      ret += node_cmd[i].front();
      node_cmd[i].pop();
    } else {
      ret += wait_s();
    }
  }
  return ret;
}

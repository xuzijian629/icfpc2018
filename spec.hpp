#include <bits/stdc++.h>
using namespace std;
using VI = vector<int>;
using VVI = vector<VI>;
using VVVI = vector<VVI>;

int R;
VVVI model;
VI query;
VI query_ans;
VI par;
int floats;

int id(int x, int y, int z) {
  return x * R * R + y * R + z;
}

VI dec(int i) {
  int x = i / (R * R);
  int y = (i % (R * R)) / R;
  int z = i % R;
  return {x, y, z};
}

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

bool is_origin(VI v) {
  return !(v[0] || v[1] || v[2]);
}

string move_from_to(int dx, int dy, int dz) {
  if (!(dx || dy || dz)) return "";
  int fdx = 0, fdy = 0, fdz = 0;
  if (dx > 15) {
    fdx = dx - 15;
    dx = 15;
  }
  if (dx < -15) {
    fdx = dx + 15;
    dx = -15;
  }
  if (dx) {
    return smove_s({dx, 0, 0}) + move_from_to(fdx, dy, dz);
  }

  if (dy > 15) {
    fdy = dy - 15;
    dy = 15;
  }
  if (dy < -15) {
    fdy = dy + 15;
    dy = -15;
  }
  if (dy) {
    return smove_s({0, dy, 0}) + move_from_to(dx, fdy, dz);
  }

  if (dz > 15) {
    fdz = dz - 15;
    dz = 15;
  }
  if (dz < -15) {
    fdz = dz + 15;
    dz = -15;
  }
  if (dz) {
    return smove_s({0, 0, dz}) + move_from_to(dx, dy, fdz);
  }

  exit(1);
}

string move_from_to(VI curpos, VI nxtpos) {
  VI d(3);
  for (int i = 0; i < 3; i++) {
    d[i] = nxtpos[i] - curpos[i];
  }
  return move_from_to(d[0], d[1], d[2]);
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

int max_y;

void read_binary(string s) {
  FILE *fp;
  if ((fp = fopen(s.c_str(), "rb")) == NULL) {
    cerr << "Failed to open file" << endl;
    exit(1);
  }

  uint8_t r;
  fread(&r, sizeof(uint8_t), 1, fp);
  R = int(r);
  par = VI(R * R * R);
  for (int i = 0; i < R * R * R; i++) {
    par[i] = i;
  }
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
        if (model[i][j][k]) {
          max_y = max(max_y, j);
          if (j == 0) {
            par[id(i, j, k)] = -1;
          }
        }
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
  uint8_t c[32000000];
  assert(cmd.size() / 8 < 32000000);
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

int find(int x) {
  if (x == -1) return -1;
  if (par[x] == -1) return -1;
  if (par[x] == x) return x;
  return par[x] = find(par[x]);
}

void unite(int x, int y) {
  x = find(x);
  y = find(y);
  if (x != y) {
    if (x == -1) {
      par[y] = -1;
      floats--;
      return;
    }
    if (y == -1) {
      par[x] = -1;
      floats--;
      return;
    }
    par[x] = y;
    floats--;
  }
}

bool ungrounded(int x, int y, int z) {
  if (y == 0) return false;
  int v = id(x, y, z);
  VI d[6] = {{0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}};
  map<int, int> parv;
  for (int i = 0; i < 6; i++) {
    int nx = x + d[i][0];
    int ny = y + d[i][1];
    int nz = z + d[i][2];
    if (model[nx][ny][nz] == 2) {
      parv[par[id(nx, ny, nz)]]++;
    }
  }
  if (parv.size() == 0) {
    floats++;
    return true;
  }

  int front = (*parv.begin()).first;
  par[v] = front;
  for (auto p: parv) {
    unite(p.first, front);
  }

  assert (floats >= 0);
  return floats != 0;
}

void solve_query() {
  int n = query.size();
  query_ans = VI(n);
  VI d[6] = {{0, 0, 1}, {0, 0, -1}, {0, 1, 0}, {0, -1, 0}, {1, 0, 0}, {-1, 0, 0}};
  VI visited(R * R * R);
  for (int i = n - 1; i >= 0; i--) {
    int v = query[i];
    visited[v] = true;
    VI c = dec(v);
    if (c[1] == 0) {
      query_ans[i] = true;
      par[v] = -1;
      continue;
    }

    map<int, int> parv;
    for (int j = 0; j < 6; j++) {
      int nx = c[0] + d[j][0];
      int ny = c[1] + d[j][1];
      int nz = c[2] + d[j][2];
      if (visited[id(nx, ny, nz)]) {
        parv[par[id(nx, ny, nz)]]++;
      }
    }
    if (parv.size() == 0) {
      query_ans[i] = floats == 0;
      floats++;
      continue;
    }

    query_ans[i] = floats == 0;
    int front = (*parv.begin()).first;
    par[v] = front;
    for (auto p: parv) {
      unite(p.first, front);
    }
  }
}

enum Axis { x_axis, y_axis, z_axis };
string long_move_s(int from, int to, Axis ax) {
  string ans = "";
  while (from != to) {
    int dist = max(-15, min(15, to - from));
    switch(ax) {
      case x_axis: ans += smove_s({dist, 0, 0}); break;
      case y_axis: ans += smove_s({0, dist, 0}); break;
      case z_axis: ans += smove_s({0, 0, dist}); break;
    }
    from += dist;
  }
  return ans;
}

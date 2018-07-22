#include <bits/stdc++.h>
#include <ctime>
#include <unistd.h>
using namespace std;
using VI = vector<int>;
using VVI = vector<VI>;
using VVVI = vector<VVI>;

VI dx{1, 0, -1, 0, 0, 0};
VI dy{0, 0, 0, 0, 1, -1};
VI dz{0, 1, 0, -1, 0, 0};
VVVI model;
int R;
const int kDfsNormal = 6;
const int kDfsUpper = 5;
int dfs_type = kDfsNormal;

bool isAllErased()
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < R; j++)
        {
            for (int k = 0; k < R; k++)
            {
                if (model[i][j][k] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void eraseComponent(int x, int y, int z)
{

    if (x < 0 || x >= R || y < 0 || y >= R || z < 0 || z >= R)
        return;
    if (model[x][y][z] == 0)
        return;

    model[x][y][z] = 0;
    for (int i = 0; i < dfs_type; i++)
    {
        eraseComponent(x + dx[i], y + dy[i], z + dz[i]);
    }

    return;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cerr << "第一引数に.mdlファイルを指定して下さい。" << std::endl;
    }

    if (argc == 3)
    {
        dfs_type = kDfsUpper;
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "rb")) == NULL)
    {
        cerr << "Failed to open file" << endl;
        exit(1);
    }

    uint8_t r;
    fread(&r, sizeof(uint8_t), 1, fp);
    R = int(r);
    model = VVVI(R, VVI(R, VI(R)));
    string bin = "";
    uint8_t *c = reinterpret_cast<uint8_t *>(calloc(int(R * R * R / 8.0 + 1), 1));
    fread(c, sizeof(uint8_t), int(ceil(R * R * R / 8.0)), fp);
    fclose(fp);
    cerr.flush();
    for (int i = 0; i < int(ceil(R * R * R / 8.0)); i++)
    {
        for (int bit = 0; bit < 8; bit++)
            bin += '0' + !!(c[i] & 1 << bit);
    }
    free(c);

    for (int i = 0; i < R; i++)
        for (int j = 0; j < R; j++)
            for (int k = 0; k < R; k++)
                model[i][j][k] = int(bin[i * R * R + j * R + k] == '1');

    int ComponentCount = 0;
    for (int x = 0; x < R; x++)
    {
        for (int z = 0; z < R; z++)
        {
            if (model.at(x).at(0).at(z) == 1)
            {
                eraseComponent(x, 0, z);
                ComponentCount++;
            }
        }
    }

    cerr << argv[1];
    switch (dfs_type)
    {
    case kDfsNormal:
        if (!isAllErased())
            cerr << "ERROR!!";
        else if (ComponentCount != 1)
            cerr << " disComponent:" << ComponentCount;
        break;
    case kDfsUpper:
        if (!isAllErased())
        {
            cerr << "has down tail";
        }
    }
    cerr << endl;
    return 0;
}

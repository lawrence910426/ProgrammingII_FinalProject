#include "include/Model.h"
#include "include/Tensor.h"

#include "SRS.h"

#include <numeric>
#include <iomanip>

struct tetro { bool data[4][4]; };

class tetromino {
    int type;
public:
    tetromino(int t): type(t) {}
    tetro Get(int direction) {
        tetro ans;
        memcpy(ans.data, SRS[type][direction], sizeof(ans));
        return ans;
    }
};

class board {
    bool data[20][10], previous[20][10];
public:
    bool drop_tetro(tetro T, int offset) {
        memcpy(previous, data, sizeof(data));
        int dropdown = 0;
        bool invalid = true;
        for(dropdown = 0;dropdown < 20;dropdown += 1) {
            for(int x = 0;x < 4;x++) for(int y = 0;y < 4;y++) if(T.data[x][y])
                if(x + dropdown >= 20 || y + offset >= 10 || data[x + dropdown][y + offset])
                    goto fail;

            invalid = false; continue;
            fail: break;
        }
        if(invalid) return true;
        for(int x = 0;x < 4;x++) for(int y = 0;y < 4;y++) if(T.data[x][y]) data[x + dropdown][y + offset] = true;
    }
    void revert_tetro() {
        memcpy(data, previous, sizeof(data));
    }
    std::vector<double> get_encoded() {
        std::vector<double> ans = std::vector<double>(20 * 10);
        for(int i = 0;i < 20;i++) for(int j = 0;j < 10;j++) ans[i * 10 + j] = (data[i][j] ? 1.0 : 0.0);
        return ans;
    }
};

class ReinLearn {
    Model *model;
    Tensor *input, *output;
public:
    ReinLearn() {
        this->model = new Model("../../pb/load_model.pb");
        this->input = new Tensor{*model, "s1"};
        this->output = new Tensor{*model, "eval_1"};
    }

    std::vector<double> encode(board* B, tetromino* T) {
        std::vector<double> data, temp, failed;
        for(int direction = 0; direction < 4; direction++) {
            for(int offset = 0; offset < 10; offset++) {
                bool fail = B->drop_tetro(T->Get(direction), offset);
                temp = B->get_encoded();
                if(!fail) B->revert_tetro();
                data.insert(data.end(), temp.begin(), temp.end());
                failed.push_back(fail ? 1.0 : 0.0);
            }
        }
        data.insert(data.end(), failed.begin(), failed.end());
        return data;
        /* data.length = 20 * 10 * 4 * 10 + 1 * 10 * 4 */
    }

    int Get(board* B, tetromino* T) {
        auto in = encode(B, T);
        input->set_data(in);
        model->run({input}, output);
        std::vector<double> out = output->get_data<double>();
        int argmax = 0;
        for(int i = 0;i < out.size();i++)
            if(abs(in[i + 20 * 10 * 4 * 10]) < 1e-6 && out[argmax] < out[i])
                argmax = i;
        return argmax;
    }
};

int main() {

}

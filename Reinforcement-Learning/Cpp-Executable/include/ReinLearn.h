//
// Created by root on 2021/1/13.
//

#include <assert.h>
#include <vector>
#include "Model.h"
#include "Tensor.h"
#include "SRS.h"

struct tetro { bool data[4][4]; int type; };

class tetromino {
    int type;
public:
    tetromino(int t): type(t) {}
    tetro Get(int direction) {
        tetro ans;
        memcpy(ans.data, SRS[type][direction], sizeof(ans));
        ans.type = type;
        return ans;
    }
};

class board {
    std::vector<std::vector<bool> > data, previous;
public:
    std::vector<std::vector<int> > show, prev;
    board() {
        show = std::vector<std::vector<int> >(20, std::vector<int>(10, 1));
        prev = std::vector<std::vector<int> >(20, std::vector<int>(10, 1));
        data = std::vector<std::vector<bool> >(20, std::vector<bool>(10, false));
        previous = std::vector<std::vector<bool> >(20, std::vector<bool>(10, false));
    }
    bool drop_tetro(tetro T, int offset) {
        previous = data; prev = show;
        int dropdown = 0;
        for(dropdown = 0;dropdown < 20;dropdown += 1) {
            for(int x = 0;x < 4;x++) for(int y = 0;y < 4;y++) if(T.data[x][y])
                        if(x + dropdown >= 20 || y + offset >= 10 || data[x + dropdown][y + offset])
                            goto fail;
        }
        fail:
        if(dropdown == 0) return true;
        dropdown -= 1;
        for(int x = 0;x < 4;x++) for(int y = 0;y < 4;y++) if(T.data[x][y]) {
            data[x + dropdown][y + offset] = true;
            show[x + dropdown][y + offset] = T.type + 2;
        }
        for(int i = 0;i < 20;i++) {
            bool line = true;
            for(int j = 0;j < 10;j++) line &= data[i][j];
            if(line) {
                for(int ii = i;ii >= 1;i--) for(int jj = 0;jj < 10;jj++) {
                    data[ii][jj] = data[ii - 1][jj];
                    show[ii][jj] = show[ii - 1][jj];
                }
                for(int jj = 0;jj < 10;jj++) {
                    data[0][jj] = false;
                    show[0][jj] = 1;
                }
            }
        }

        for(int i = 0;i < 20;i++) for(int j = 0;j < 10;j++) if(!((data[i][j] && show[i][j] != 1) || (!data[i][j] && show[i][j] == 1)))
            assert(false);
    }
    void revert_tetro() { data = previous; show = prev; }
    std::vector<double> get_encoded() {
        std::vector<double> ans = std::vector<double>(20 * 10);
        for(int i = 0;i < 20;i++) for(int j = 0;j < 10;j++) ans[i * 10 + j] = (data[i][j] ? 1.0 : 0.0);
        return ans;
    }
};

class ReinLearn {
    Model *model;
    Tensor *input, *output;

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

public:
    ReinLearn() {
        this->model = new Model("../../pb/load_model.pb");
        this->input = new Tensor{*model, "s1"};
        this->output = new Tensor{*model, "eval_1"};
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
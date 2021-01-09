#include "include/Model.h"
#include "include/Tensor.h"

#include <numeric>
#include <iomanip>

int main() {
    // Load model with a path to the .pb file.
    // An optional std::vector<uint8_t> parameter can be used to supply Tensorflow with
    // session options. The vector must represent a serialized ConfigProto which can be
    // generated manually in python. See create_config_options.py.
    // Example:
    // const std::vector<uint8_t> ModelConfigOptions = { 0x32, 0xb, 0x9, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xb9, 0x3f, 0x20, 0x1 };
    // Model model("../model.pb", ModelConfigOptions);
    Model model("../../pb/load_model.pb");
    // model.init();

    Tensor input_a{model, "s1"};
    Tensor output{model, "eval_1"};

    std::vector<double> data(100);
    input_a.set_data(data);

    model.run({&input_a}, output);
    for (double f : output.get_data<double>()) {
        std::cout << f << " ";
    }
    std::cout << std::endl;

}

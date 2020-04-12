#include "Model.hpp"
#include "spdlog/fmt/fmt.h"

namespace ES {
Model::Model(std::size_t id) :
    fullPath_{fmt::format("{}/model{}.txt", directoryPath_, id)},
    file_{fullPath_} {
    HandleFileRead();
}

Model::Type Model::GetModel() const {
    return model_;
}

void Model::HandleFileRead() {
    try {
        file_.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        ReadFromFile();
    } catch (const std::ifstream::failure& exception) {
        fmt::print("Exception: \"{}\" during opening/reading {}\n",
                   exception.what(),
                   fullPath_);
    }
}

void Model::ReadFromFile() {
    for (std::string line; file_.peek() != EOF && std::getline(file_, line);) {
        std::stringstream lineStream{line};
        double value = 0.0;
        for (auto& vector : model_) {
            lineStream >> value;
            vector.push_back(value);
        }
    }
}
} // namespace ES
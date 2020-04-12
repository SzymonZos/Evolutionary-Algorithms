#ifndef EVOLUTIONARYALGORITHMS_MODEL_HPP
#define EVOLUTIONARYALGORITHMS_MODEL_HPP

#include "spdlog/fmt/fmt.h"
#include <filesystem>
#include <fstream>

namespace ES {
enum ModelIO : std::size_t { in, out };

class Model {
public:
    using Type = std::array<std::vector<double>, 2>;
    Model() = delete;
    ~Model() = default;

    Model(const Model& other) = delete;
    Model& operator=(const Model& other) = delete;

    Model(Model&& other) = delete;
    Model& operator=(Model&& other) = delete;

    explicit Model(std::size_t id);
    std::array<std::vector<double>, 2> GetModel() const;

private:
    static constexpr const char* directoryPath_ = PROJECT_SOURCE_DIR "/models";
    std::string fullPath_;
    std::ifstream file_;
    Type model_{};

    void HandleFileRead();
    void ReadFromFile();
};
} // namespace ES

#endif // EVOLUTIONARYALGORITHMS_MODEL_HPP

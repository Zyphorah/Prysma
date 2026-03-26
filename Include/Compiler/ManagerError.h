#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#include <string>
#include <stdexcept>

struct Line {
private:
    int value;
public:
    explicit Line(int val) : value(val) {}
    [[nodiscard]] auto getValue() const -> int { return value; }
};

struct Column {
private:
    int value;
public:
    explicit Column(int val) : value(val) {}
    [[nodiscard]] auto getValue() const -> int { return value; }
};

class CompilationError : public std::runtime_error
{
private:
    Line line;
    Column column;

public:
    [[nodiscard]] auto getLine() const -> int { return line.getValue(); }
    [[nodiscard]] auto getColumn() const -> int { return column.getValue(); }

    CompilationError(const std::string& message, Line lin, Column col)
        : std::runtime_error(message), line(lin), column(col)
    {
    }

    CompilationError(const CompilationError&) = default;
    auto operator=(const CompilationError&) -> CompilationError& = default;
    CompilationError(CompilationError&&) = default;
    auto operator=(CompilationError&&) -> CompilationError& = default;

    ~CompilationError() override = default;
};

#endif /* ERROR_MANAGER_H */

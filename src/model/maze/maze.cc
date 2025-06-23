#include "maze.h"

std::mt19937 Maze::_gen;
std::uniform_int_distribution<> Maze::_dist_bit(0, 1);
std::uniform_real_distribution<> Maze::_dist_real(0.0, 1.0);

void Maze::InitRandom() {
  static bool initialized = false;
  if (!initialized) {
    _gen.seed(std::random_device{}());
    initialized = true;
  }
}

bool Maze::SetRowsCols(int rows, int cols) {
  if (rows > 0 && cols > 0 && rows <= kMaxSize && cols <= kMaxSize) {
    _rows = rows;
    _cols = cols;
    return true;
  }
  return false;
}

Maze::Maze() : _rows(0), _cols(0) {
  _verticals.fill(0);
  _horizontals.fill(0);
}

Maze::Maze(int rows, int cols) {
  if (SetRowsCols(rows, cols)) {
    _verticals.fill(0);
    _horizontals.fill(0);
  } else {
    throw std::invalid_argument("Invalid maze dimensions");
  }
}

bool Maze::Load(std::istream& stream, char c) {
  if (stream) {
    std::string line;
    while (getline(stream, line) && line.empty());
    std::istringstream tmp(line);
    if (tmp >> _rows >> _cols) {
      if ((_rows > 0 && _cols > 0 && _rows <= kMaxSize && _cols <= kMaxSize)) {
        return LoadMatrices(stream, c);
      }
      return false;
    }
  }
  return false;
}

bool Maze::LoadMatrices(std::istream& stream, char c) {
  bool result = LoadMatrix(stream, 'c');
  if (c == 'm') result &= LoadMatrix(stream, 'm');
  return result;
}

bool Maze::LoadMatrix(std::istream& stream, char c) {
  int i = 0;
  std::string line;
  while (i < _rows && std::getline(stream, line)) {
    if (!line.empty()) {
      std::istringstream tmp(line);
      std::string token;
      int j = 0;
      while (j < _cols && tmp >> token) {
        if (token == "1") {
          if (c == 'c')
            SetBit1(_verticals[i], j);
          else
            SetBit1(_horizontals[i], j);
        } else if (token == "0") {
          if (c == 'c')
            SetBit0(_verticals[i], j);
          else
            SetBit0(_horizontals[i], j);
        } else {
          return false;
        }
        ++j;
      }
      if (j < _cols) return false;
      ++i;
    }
  }
  if (i < _rows) return false;
  return true;
}

bool Maze::Save(std::ostream& stream, char c) const {
  if (stream) {
    stream << _rows << ' ' << _cols << '\n';
    bool result = SaveMatrix(stream, 'c');
    if (c == 'm') result &= SaveMatrix(stream, 'm');
    return result;
  }
  return false;
}

bool Maze::SaveMatrix(std::ostream& stream, char c) const {
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      if (c == 'c')
        stream << GetBit(_verticals[i], j) << ' ';
      else
        stream << GetBit(_horizontals[i], j) << ' ';
    }
    stream << '\n';
  }
  stream << '\n';
  return stream.good();
}

void Maze::SetBit1(uint64_t& n, int pos) { n |= ((uint64_t)1 << pos); }
void Maze::SetBit0(uint64_t& n, int pos) { n &= ~((uint64_t)1 << pos); }

void Maze::GenerateMaze() {
  std::array<int, kMaxSize> set;
  int uniques = 1;
  for (int j = 0; j < _cols; ++j) {
    set[j] = uniques++;
  }
  for (int i = 0; i < _rows - 1; ++i) {
    MakeVerticalWalls(set, i);
    for (int j = 0; j < _cols; ++j) {
      RandomBit() ? SetBit1(_horizontals[i], j) : SetBit0(_horizontals[i], j);
    }
    CheckhorizontalPass(set, i, uniques);
    for (int j = 0; j < _cols; ++j) {
      if (GetBit(_horizontals[i], j)) {
        set[j] = uniques++;
      }
    }
  }
  MakeLastLine(set);
}

void Maze::MakeVerticalWalls(std::array<int, kMaxSize>& set, int i) {
  for (int j = 0; j < _cols - 1; ++j) {
    if (set[j] == set[j + 1]) {
      SetBit1(_verticals[i], j);
    } else {
      RandomBit() ? SetBit1(_verticals[i], j) : SetBit0(_verticals[i], j);
    }
    if (!GetBit(_verticals[i], j)) {
      int changing_set = set[j + 1];
      for (int k = 0; k < _cols; ++k) {
        if (set[k] == changing_set) set[k] = set[j];
      }
    }
  }
  SetBit1(_verticals[i], _cols - 1);
}

void Maze::MakeLastLine(std::array<int, kMaxSize>& set) {
  MakeVerticalWalls(set, _rows - 1);
  for (int j = 0; j < _cols - 1; ++j) {
    if (set[j] != set[j + 1]) {
      SetBit0(_verticals[_rows - 1], j);

      int changing_set = set[j + 1];
      for (int k = 0; k < _cols; ++k) {
        if (set[k] == changing_set) set[k] = set[j];
      }
    }
    SetBit1(_horizontals[_rows - 1], j);
  }
  SetBit1(_horizontals[_rows - 1], _cols - 1);
}

void Maze::CheckhorizontalPass(const std::array<int, kMaxSize>& set, int i,
                               int uniques) {
  for (int s = 1; s < uniques; ++s) {
    std::array<int, kMaxSize> cells = {0};
    int count = 0;

    for (int j = 0; j < _cols; ++j)
      if (set[j] == s) {
        cells[count++] = j;
      }
    if (count) {
      bool pass_found = false;
      for (int k = 0; k < count && !pass_found; ++k) {
        if (!GetBit(_horizontals[i], cells[k])) pass_found = true;
      }
      if (!pass_found) {
        std::uniform_int_distribution<int> dist_index(0, count - 1);
        int random_index = dist_index(_gen);
        SetBit0(_horizontals[i], cells[random_index]);
      }
    }
  }
}

// void Maze::PrintMatrixBits() const {
//   std::cout << "Vertical walls (_verticals):\n";
//   for (int i = 0; i < _rows; ++i) {
//     for (int j = 0; j < _cols; ++j) {
//       std::cout << GetBit(_verticals[i], j);
//     }
//     std::cout << '\n';
//   }

//   std::cout << "\nHorizontal walls (_horizontals):\n";
//   for (int i = 0; i < _rows; ++i) {
//     for (int j = 0; j < _cols; ++j) {
//       std::cout << GetBit(_horizontals[i], j);
//     }
//     std::cout << '\n';
//   }
// }

// void Maze::PrintMatrixValues() const {
//   std::cout << "Vertical walls (_verticals) numeric values:\n";
//   for (int i = 0; i < _rows; ++i) {
//     std::cout << _verticals[i] << '\n';
//   }

//   std::cout << "\nHorizontal walls (_horizontals) numeric values:\n";
//   for (int i = 0; i < _rows; ++i) {
//     std::cout << _horizontals[i] << '\n';
//   }
// }

Maze::Maze(const Maze& other)
    : _rows(other._rows),
      _cols(other._cols),
      _verticals(other._verticals),
      _horizontals(other._horizontals) {}

Maze& Maze::operator=(const Maze& other) noexcept {
  if (this != &other) {
    _rows = other._rows;
    _cols = other._cols;
    _verticals = other._verticals;
    _horizontals = other._horizontals;
  }
  return *this;
}

Maze::Maze(Maze&& other) noexcept
    : _rows(other._rows),
      _cols(other._cols),
      _verticals(std::move(other._verticals)),
      _horizontals(std::move(other._horizontals)) {
  other._rows = 0;
  other._cols = 0;
}

Maze& Maze::operator=(Maze&& other) noexcept {
  if (this != &other) {
    _rows = other._rows;
    _cols = other._cols;
    _verticals = std::move(other._verticals);
    _horizontals = std::move(other._horizontals);
    other._rows = 0;
    other._cols = 0;
  }
  return *this;
}

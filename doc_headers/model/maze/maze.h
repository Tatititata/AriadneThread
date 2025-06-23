/**
 * @file maze.h
 * @brief Maze class for generating, loading, saving, and solving mazes and
 * caves.
 */

#ifndef MAZE_H_
#define MAZE_H_

#include <array>
#include <cstdint>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "../cell.h"

/// Maximum allowed maze size per side.
constexpr int kMaxSize = 50;

/**
 * @class Maze
 * @brief Class for working with mazes and caves.
 *
 * Provides methods for generating, saving, loading, and solving mazes and
 * caves.
 */
class Maze {
  friend class QLearning;

 public:
  /// Default constructor.
  Maze();

  /// Constructor with specified dimensions.
  /// @param rows Number of rows.
  /// @param cols Number of columns.
  explicit Maze(int rows, int cols);

  /// Copy constructor.
  Maze(const Maze &);

  /// Copy assignment operator.
  Maze &operator=(const Maze &other) noexcept;

  /// Move constructor.
  Maze(Maze &&other) noexcept;

  /// Move assignment operator.
  Maze &operator=(Maze &&other) noexcept;

  /// Generate a new maze.
  void GenerateMaze();

  /// Generate a cave with the given fill probability.
  /// @param chance Probability for a cell to be filled (0 to 1).
  void GenerateCave(const double chance);

  /// Perform one evolution step for the cave using birth/death rules.
  /// @param birth Birth threshold.
  /// @param death Death threshold.
  /// @return true if the cave is stabilized.
  bool SolveCave(const int birth, const int death);

  /// Solve the maze from start to end.
  /// @param end End cell.
  /// @param start Start cell.
  /// @return Vector of cells representing the path.
  std::vector<Cell> SolveMaze(Cell end, Cell start);

  /// Build a distance matrix from the given cell.
  /// @param start Start cell.
  /// @return Matrix of distances.
  std::vector<std::vector<Cell>> DistanceMatrix(Cell start);

  /// Load a maze from a stream.
  /// @param stream Input stream.
  /// @param c Mode character ('c' for verticals, 'm' for horizontals).
  /// @return true if loading was successful.
  bool Load(std::istream &stream, char c);

  /// Save the maze to a stream.
  /// @param stream Output stream.
  /// @param c Mode character ('c' for verticals, 'm' for horizontals).
  /// @return true if saving was successful.
  bool Save(std::ostream &stream, char c) const;

  /// Get number of rows.
  /// @return Number of rows.
  int GetRows() const { return _rows; }

  /// Get number of columns.
  /// @return Number of columns.
  int GetCols() const { return _cols; }

  /// Get the value of a bit at a given position.
  /// @param n Number.
  /// @param bit Bit position.
  /// @return Value of the bit.
  static int GetBit(uint64_t n, int bit) { return (n >> bit) & 1u; }

  /// Get the array of vertical walls.
  /// @return Array of vertical walls.
  std::array<uint64_t, kMaxSize> GetVerticals() const { return _verticals; }

  /// Get the array of horizontal walls.
  /// @return Array of horizontal walls.
  std::array<uint64_t, kMaxSize> GetHorizontals() const { return _horizontals; }

  /// Set the array of vertical walls.
  /// @param verticals New array of verticals.
  void SetVerticals(std::array<uint64_t, kMaxSize> verticals) {
    _verticals = verticals;
  }

  /// Set the array of horizontal walls.
  /// @param horizontals New array of horizontals.
  void SetHorizontals(std::array<uint64_t, kMaxSize> horizontals) {
    _horizontals = horizontals;
  }

  /// Set maze dimensions.
  /// @param rows Number of rows.
  /// @param cols Number of columns.
  /// @return true if dimensions are valid.
  bool SetRowsCols(int rows, int cols);

  /// Initialize the random number generator.
  static void InitRandom();

  /// Random number generator.
  static std::mt19937 _gen;

 private:
  /**
   * @brief Load a single wall matrix (vertical or horizontal) from a stream.
   * @param stream Input stream.
   * @param c Mode character ('c' or 'm').
   * @return true if the matrix was loaded successfully.
   */
  bool LoadMatrix(std::istream &stream, char c);

  /**
   * @brief Load both wall matrices from a stream.
   * @param stream Input stream.
   * @param c Mode character.
   * @return true if matrices were loaded successfully.
   */
  bool LoadMatrices(std::istream &stream, char c);

  /**
   * @brief Save a single wall matrix (vertical or horizontal) to a stream.
   * @param stream Output stream.
   * @param c Mode character ('c' or 'm').
   * @return true if the matrix was saved successfully.
   */
  bool SaveMatrix(std::ostream &stream, char c) const;

  /**
   * @brief Finalize the last line of the maze during generation.
   * @param set Array of set identifiers for the cells.
   */
  void MakeLastLine(std::array<int, kMaxSize> &set);

  /**
   * @brief Ensure that each set in the current row has at least one passage to
   * the next row.
   * @param set Array of set identifiers for the cells.
   * @param i Current row index.
   * @param uniques Number of unique sets.
   */
  void CheckhorizontalPass(const std::array<int, kMaxSize> &set, int i,
                           int uniques);

  /**
   * @brief Randomly create or remove vertical walls between cells in the
   * current row.
   * @param set Array of set identifiers for the cells.
   * @param i Current row index.
   */
  void MakeVerticalWalls(std::array<int, kMaxSize> &set, int i);

  /**
   * @brief Set a specific bit to 1 in a 64-bit integer.
   * @param n Reference to the integer.
   * @param pos Bit position.
   */
  static void SetBit1(uint64_t &n, int pos);

  /**
   * @brief Set a specific bit to 0 in a 64-bit integer.
   * @param n Reference to the integer.
   * @param pos Bit position.
   */
  static void SetBit0(uint64_t &n, int pos);

  /**
   * @brief Check if movement from start to end cell is possible.
   * @param start Starting cell.
   * @param end Ending cell.
   * @return true if movement is possible.
   */
  inline bool CanGo(const Cell &start, const Cell &end) const;

  /**
   * @brief Check if the cell coordinates are valid within the maze.
   * @param point Cell to check.
   * @return true if the cell is valid.
   */
  inline bool ValidPoint(const Cell &point) const;

  /**
   * @brief Check if the cell is empty (uninitialized).
   * @param point Cell to check.
   * @return true if the cell is empty.
   */
  static bool EmptyPoint(const Cell &point);

  /**
   * @brief Update the cell state for a dead cell in cave evolution.
   * @param i Row index.
   * @param j Column index.
   * @param birth Birth threshold.
   */
  void ProceedDead(const int i, const int j, const int birth);

  /**
   * @brief Update the cell state for a live cell in cave evolution.
   * @param i Row index.
   * @param j Column index.
   * @param death Death threshold.
   */
  void ProceedAlive(const int i, const int j, const int death);

  /// Number of rows in the maze.
  int _rows;

  /// Number of columns in the maze.
  int _cols;

  /// Vertical wall matrix.
  std::array<uint64_t, kMaxSize> _verticals;

  /// Horizontal wall matrix.
  std::array<uint64_t, kMaxSize> _horizontals;

  /// Distribution for random bits.
  static std::uniform_int_distribution<> _dist_bit;

  /// Distribution for random real numbers.
  static std::uniform_real_distribution<> _dist_real;

  /**
   * @brief Generate a random bit (0 or 1).
   * @return Random bit.
   */
  static int RandomBit() { return _dist_bit(_gen); }

  /**
   * @brief Generate a random real number in [0, 1].
   * @return Random real value.
   */
  static double RandomReal() { return _dist_real(_gen); }
};

#endif  // MAZE_H_

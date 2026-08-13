#ifndef MTTI2T_DATA_STRUCTURES_UNION_FIND_H_
#define MTTI2T_DATA_STRUCTURES_UNION_FIND_H_

#include <new>
#include <optional>

namespace mtti2t {
  class UnionFind {
    int * parent_;
    int * rank_;
    int set_count_;

  public:
    UnionFind(const UnionFind &) = delete;
    UnionFind &operator = (const UnionFind &) = delete;

    static std::optional < UnionFind > Construct(int set_count_) noexcept {
      int * parent = new(std::nothrow) int[set_count_];
      int * rank = new(std::nothrow) int[set_count_];

      if (parent == nullptr || rank == nullptr) {
        delete[] parent;
        delete[] rank;

        return std::nullopt;
      }

      for (int index = 0; index < set_count_; ++index) {
        parent[index] = index;
        rank[index] = 0;
      }

      return UnionFind(parent, rank, set_count_);
    }

    UnionFind(UnionFind &&other) noexcept {
      parent_ = other.parent_;
      rank_ = other.rank_;
      set_count_ = other.set_count_;
      other.parent_ = nullptr;
      other.rank_ = nullptr;
    }

    ~UnionFind() {
      Destroy();
    }

    UnionFind &operator = (UnionFind &&other) noexcept {
      if (this != &other) {
        Destroy();
        parent_ = other.parent_;
        rank_ = other.rank_;
        set_count_ = other.set_count_;
        other.parent_ = nullptr;
        other.rank_ = nullptr;
      }

      return *this;
    }

    int Find(int index) noexcept {
      int root = index;

      while (parent_[root] != root) {
        root = parent_[root];
      }

      while (parent_[index] != root) {
        int next = parent_[index];

        parent_[index] = root;
        index = next;
      }

      return root;
    }

    void Union(int first, int last) noexcept {
      int root_first = Find(first);
      int root_last = Find(last);

      if (root_first != root_last) {
        if (rank_[root_first] < rank_[root_last]) {
          parent_[root_first] = root_last;
        }
        else if (rank_[root_first] > rank_[root_last]) {
          parent_[root_last] = root_first;
        }
        else {
          parent_[root_last] = root_first;
          ++rank_[root_first];
        }

        --set_count_;
      }
    }

  private:
    UnionFind(int * parent, int * rank, int set_count) noexcept {
      parent_ = parent;
      rank_ = rank;
      set_count_ = set_count;
    }

    void Destroy() noexcept {
      delete[] parent_;
      delete[] rank_;
    }
  };
}

#endif
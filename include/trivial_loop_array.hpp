#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>

namespace theapx {

template <class T, size_t kCapacity, class Enable = void>
class trivial_loop_array {};  // primary template

template <class T, size_t kCapacity>
class trivial_loop_array<
    T, kCapacity, typename std::enable_if<std::is_trivial<T>::value>::type> {
 public:
  using value_type = T;

  template <class Owner, class P>
  class iterator_base {
   public:
    P& operator*() { return owner_->at(pos_); }
    P* operator->() { return &owner_->at(pos_); }

    iterator_base<Owner, P> operator--() {
      --pos_;
      return *this;
    }
    iterator_base<Owner, P> operator--(int) {
      iterator_base<Owner, P> result = *this;
      --pos_;
      return result;
    }
    iterator_base<Owner, P> operator++() {
      ++pos_;
      return *this;
    }
    iterator_base<Owner, P> operator++(int) {
      iterator_base<Owner, P> result = *this;
      ++pos_;
      return result;
    }

    bool operator==(const iterator_base<Owner, P>& other) const {
      return this->owner_ == other.owner_ && this->pos_ == other.pos_;
    }
    bool operator!=(const iterator_base<Owner, P>& other) const {
      return !operator==(other);
    }

    iterator_base(const iterator_base<Owner, P>& other)
        : owner_(other.owner_), pos_(other.pos_) {}
    iterator_base(Owner* owner, size_t pos) : owner_(owner), pos_(pos) {}

   private:
    Owner* owner_;
    int pos_;
  };

  using iterator = iterator_base<trivial_loop_array, T>;
  using const_iterator = iterator_base<const trivial_loop_array, const T>;

  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, array_size); }
  const_iterator begin() const { return const_iterator(this, 0); }
  const_iterator end() const { return const_iterator(this, array_size); }

  T& at(size_t pos);
  const T& at(size_t pos) const;
  T& operator[](size_t pos) { return at(pos); }
  const T& operator[](size_t pos) const { return at(pos); }

  size_t capacity() const { return kCapacity; }
  size_t size() const { return array_size; }
  bool empty() const { return size() == 0; }

  void reserve(size_t){/*no-op*/};
  void resize(size_t new_size);

  void push_back(const T& v);
  void push_front(const T& v);

  // ---------- data ----------
  T data[kCapacity];
  size_t start_index;
  size_t array_size;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <class T, size_t kCapacity>
inline T& trivial_loop_array<
    T, kCapacity,
    typename std::enable_if<std::is_trivial<T>::value>::type>::at(size_t pos) {
  if (pos >= array_size || pos < 0) {
    throw std::out_of_range("pos");
  }
  return data[(start_index + pos) % kCapacity];
}

template <class T, size_t kCapacity>
inline const T& trivial_loop_array<
    T, kCapacity,
    typename std::enable_if<std::is_trivial<T>::value>::type>::at(size_t pos)
    const {
  if (pos >= array_size || pos < 0) {
    throw std::out_of_range("pos");
  }
  return data[(start_index + pos) % kCapacity];
}

template <class T, size_t kCapacity>
inline void trivial_loop_array<
    T, kCapacity, typename std::enable_if<std::is_trivial<T>::value>::type>::
    resize(size_t new_size) {
  if (new_size > kCapacity || new_size < 0) {
    throw std::out_of_range("new_size");
  }
  array_size = new_size;
}

template <class T, size_t kCapacity>
inline void trivial_loop_array<
    T, kCapacity, typename std::enable_if<std::is_trivial<T>::value>::type>::
    push_back(const T& v) {
  if (array_size == kCapacity) {
    throw std::length_error("trivial_loop_array capacity exceeded");
  }

  ++array_size;
  at(array_size - 1) = v;
}

template <class T, size_t kCapacity>
inline void trivial_loop_array<
    T, kCapacity, typename std::enable_if<std::is_trivial<T>::value>::type>::
    push_front(const T& v) {
  if (array_size == kCapacity) {
    throw std::length_error("trivial_loop_array capacity exceeded");
  }

  ++array_size;
  if (start_index == 0) {
    start_index = kCapacity - 1;
  } else {
    --start_index;
  }

  at(0) = v;
}

}  // namespace theapx

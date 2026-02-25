#pragma once

#include <map>
#include <plf_colony.h>
#include <unordered_map>

namespace mono {

template<class T, class Alloc = std::allocator<T>, plf::priority priority = plf::performance>
using hive = plf::colony<T, Alloc, priority>;

using limits = plf::limits;


/*
 * DeferredHive is a wrapper around the hive container that allows for deferred erasure of elements.
 * Elements marked for deletion are not immediately removed from the container,
 * but are instead scheduled for deletion after a specified number of ticks.
 *
 * @tparam T The type of elements stored in the hive.
 * @tparam Delay The number of ticks to wait before actually erasing the marked elements.
 */
template<typename T, std::uint8_t Delay>
  requires(Delay > 0)
class DeferredHive {
public:
  using value_type             = T;
  using underlying_container   = hive<value_type>;
  using iterator               = typename underlying_container::iterator;
  using const_iterator         = typename underlying_container::const_iterator;
  using reverse_iterator       = typename underlying_container::reverse_iterator;
  using const_reverse_iterator = typename underlying_container::const_reverse_iterator;
  using size_type              = typename underlying_container::size_type;
  using ticks_type             = std::uint8_t;

  DeferredHive() = default;

  [[nodiscard]] ticks_type current_tick() const { return current_tick; }

  void tick(std::uint8_t ticks = 1) {
    for (std::uint8_t i = 0; i < ticks; ++i) {
      tick_value = (tick_value + 1) % Delay;

      auto [begin, end] = delayed_deletions.equal_range(tick_value);
      auto range        = std::ranges::subrange(begin, end);

      // Erase elements that were marked for deletion in the current tick
      std::ranges::for_each(range, [this](auto& pair) { container.erase(pair.second); });

      // Erase the entries for the current tick after processing
      delayed_deletions.erase(tick_value);
    }
  }

  iterator insert(value_type&& value) { // NOLINT
    return container.insert(std::forward<value_type>(value));
  }

  iterator insert(value_type const& value) { return container.insert(value); }

  void erase(iterator const it) { delayed_deletions.emplace(tick_value, it); }

  void erase(iterator const first, iterator const last) {
    for (auto it = first; it != last; ++it) {
      delayed_deletions.emplace(tick_value, it);
    }
  }

  void clear() {
    container.clear();
    delayed_deletions.clear();
  }

  [[nodiscard]] size_type size() const noexcept { return container.size(); }

  [[nodiscard]] size_type max_size() const noexcept { return container.max_size(); }

  [[nodiscard]] size_type capacity() const noexcept { return container.capacity(); }

  [[nodiscard]] bool empty() const noexcept { return container.empty(); }

  [[nodiscard]] iterator begin() noexcept { return container.begin(); }

  [[nodiscard]] iterator end() noexcept { return container.end(); }

  [[nodiscard]] const_iterator begin() const noexcept { return container.begin(); }

  [[nodiscard]] const_iterator end() const noexcept { return container.end(); }

  [[nodiscard]] const_iterator cbegin() const noexcept { return container.cbegin(); }

  [[nodiscard]] const_iterator cend() const noexcept { return container.cend(); }

  [[nodiscard]] reverse_iterator rbegin() noexcept { return container.rbegin(); }

  [[nodiscard]] reverse_iterator rend() noexcept { return container.rend(); }

  [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return container.rbegin(); }

  [[nodiscard]] const_reverse_iterator rend() const noexcept { return container.rend(); }

  [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return container.crbegin(); }

  [[nodiscard]] const_reverse_iterator crend() const noexcept { return container.crend(); }

  void reserve(size_type const n) { container.reserve(n); }

  void reshape(limits block_limits) { container.reshape(block_limits); }

  void trim_capacity() noexcept { container.trim_capacity(); }

  void shrink_to_fit() { container.shrink_to_fit(); }

private:
  underlying_container container;
  std::unordered_multimap<ticks_type, iterator> delayed_deletions;
  ticks_type tick_value {0};
};

} // namespace mono

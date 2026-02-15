#include <compare>
#include <concepts>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>

namespace mono::views {

template<std::ranges::view V>
  requires std::ranges::forward_range<V>
class CycleView : public std::ranges::view_interface<CycleView<V>> {
private:
  V base_ = V();

  template<bool Const>
  class iterator {
    friend CycleView;
    using Base = std::conditional_t<Const, V const, V>;

    std::ranges::iterator_t<Base> current_                         = std::ranges::iterator_t<Base>();
    std::conditional_t<Const, CycleView const, CycleView>* parent_ = nullptr;
    std::ranges::range_difference_t<Base> n_                       = 0;

    constexpr iterator(
        std::conditional_t<Const, CycleView const, CycleView>& parent, std::ranges::iterator_t<Base> current
    ) : current_(std::move(current)), parent_(std::addressof(parent)) { }

  public:
    using value_type      = std::ranges::range_value_t<Base>;
    using difference_type = std::ptrdiff_t;

    using iterator_concept = std::conditional_t<
        std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>, std::random_access_iterator_tag,
        std::conditional_t<
            std::ranges::bidirectional_range<Base> && std::ranges::common_range<Base>, std::bidirectional_iterator_tag,
            std::forward_iterator_tag>>;

    using iterator_category = std::conditional_t<
        std::derived_from<
            typename std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category,
            std::random_access_iterator_tag> &&
            std::ranges::sized_range<Base>,
        std::random_access_iterator_tag,
        std::conditional_t<
            std::derived_from<
                typename std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category,
                std::bidirectional_iterator_tag> &&
                std::ranges::common_range<Base>,
            std::bidirectional_iterator_tag, std::forward_iterator_tag>>;

    iterator()
      requires std::default_initializable<std::ranges::iterator_t<Base>>
    = default;

    constexpr iterator(iterator<!Const> i)
      requires Const && std::convertible_to<std::ranges::iterator_t<V>, std::ranges::iterator_t<Base>>
      : current_(std::move(i.current_)), parent_(i.parent_), n_(i.n_) { }

    constexpr std::ranges::iterator_t<Base> base() const { return current_; }

    constexpr decltype(auto) operator*() const { return *current_; }

    constexpr iterator& operator++() {
      if (++current_ == std::ranges::end(parent_->base_)) {
        current_ = std::ranges::begin(parent_->base_);
        ++n_;
      }
      return *this;
    }

    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    constexpr iterator& operator--()
      requires std::ranges::bidirectional_range<Base> && std::ranges::common_range<Base>
    {
      if (current_ == std::ranges::begin(parent_->base_)) {
        current_ = std::ranges::end(parent_->base_);
        --n_;
      }
      --current_;
      return *this;
    }

    constexpr iterator operator--(int)
      requires std::ranges::bidirectional_range<Base> && std::ranges::common_range<Base>
    {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    constexpr iterator& operator+=(difference_type n)
      requires std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>
    {
      auto const first      = std::ranges::begin(parent_->base_);
      auto const dist       = std::ranges::distance(parent_->base_);
      auto const offset     = current_ - first;
      auto const new_n      = n + offset;
      auto const new_offset = new_n % dist;
      n_ += new_n / dist;
      current_ = first + (new_offset >= 0 ? new_offset : new_offset + dist);
      return *this;
    }

    constexpr iterator& operator-=(difference_type n)
      requires std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>
    {
      return *this += -n;
    }

    constexpr decltype(auto) operator[](difference_type n) const
      requires std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>
    {
      return *(*this + n);
    }

    friend constexpr bool operator==(iterator const& x, iterator const& y) {
      return x.n_ == y.n_ && x.current_ == y.current_;
    }

    friend constexpr bool operator==(iterator const& x, [[maybe_unused]] std::default_sentinel_t sentinel) {
      return std::ranges::empty(x.parent_->base_);
    }

    friend constexpr auto operator<=>(iterator const& x, iterator const& y)
      requires std::ranges::random_access_range<Base> && std::three_way_comparable<std::ranges::iterator_t<Base>>
    {
      if (auto cmp = x.n_ <=> y.n_; cmp != 0)
        return cmp;
      return x.current_ <=> y.current_;
    }

    friend constexpr iterator operator+(iterator const& i, difference_type n)
      requires std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>
    {
      auto r = i;
      r += n;
      return r;
    }

    friend constexpr iterator operator+(difference_type n, iterator const& i)
      requires std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>
    {
      return i + n;
    }

    friend constexpr iterator operator-(iterator const& i, difference_type n)
      requires std::ranges::random_access_range<Base> && std::ranges::sized_range<Base>
    {
      auto r = i;
      r -= n;
      return r;
    }

    friend constexpr difference_type operator-(iterator const& x, iterator const& y)
      requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>> &&
               std::ranges::sized_range<Base>
    {
      auto const dist = std::ranges::distance(x.parent_->base_);
      return ((x.n_ - y.n_) * dist) + (x.current_ - y.current_);
    }
  };

public:
  CycleView()
    requires std::default_initializable<V>
  = default;
  constexpr explicit CycleView(V base) : base_(std::move(base)) { }

  constexpr V base() const&
    requires std::copy_constructible<V>
  {
    return base_;
  }
  constexpr V base() && { return std::move(base_); }

  constexpr auto begin() { return iterator<false>(*this, std::ranges::begin(base_)); }
  constexpr auto begin() const
    requires std::ranges::forward_range<V const>
  {
    return iterator<true>(*this, std::ranges::begin(base_));
  }

  constexpr auto end() const noexcept { return std::default_sentinel; }
};

template<class R>
CycleView(R&&) -> CycleView<std::views::all_t<R>>;

// Range Adaptor
struct CycleFn {
  template<std::ranges::viewable_range R>
    requires std::ranges::forward_range<R>
  constexpr auto operator()(R&& r) const {
    return CycleView(std::views::all(std::forward<R>(r)));
  }

  template<std::ranges::forward_range R>
  friend constexpr auto operator|(R&& r, CycleFn fn) {
    return fn(std::forward<R>(r));
  }
};

inline constexpr CycleFn cycle;

} // namespace mono::views

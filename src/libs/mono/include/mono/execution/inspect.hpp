#pragma once

#include <exec/completion_signatures.hpp>
#include <stdexec/execution.hpp>

#include <exception>
#include <utility>

namespace mono::ex {

namespace detail {

template<class R, class F>
class InspectReceiver {
public:
  using receiver_concept = stdexec::receiver_t;

  InspectReceiver(R r, F f) : receiver(std::move(r)), func(std::move(f)) { }

  template<class... As>
    requires(std::invocable<F, As...>)
  void set_value(As&&... as) noexcept try {
    std::invoke(func, as...);
    stdexec::set_value(std::move(receiver), std::forward<As>(as)...);
  }
  catch (...) {
    stdexec::set_error(std::move(receiver), std::current_exception());
  }

  template<class E>
  void set_error(E&& e) noexcept {
    stdexec::set_error(std::move(receiver), std::forward<E>(e));
  }

  void set_stopped() noexcept { stdexec::set_stopped(std::move(receiver)); }

  decltype(auto) get_env() const noexcept { return stdexec::get_env(receiver); }

private:
  R receiver;
  F func;
};

template<stdexec::sender Child, class F>
struct InspectSender {
  using sender_concept = stdexec::sender_t;

  template<class Self, class... Env>
  static consteval auto get_completion_signatures() noexcept {
    auto child_completions = exec::get_child_completion_signatures<Self, Child, Env...>();
    auto value_fn          = []<class... Args>() {
      return stdexec::completion_signatures<
                   stdexec::set_value_t(Args...), //
                   stdexec::set_error_t(std::exception_ptr) //
                   >();
    };

    return exec::transform_completion_signatures(child_completions, value_fn);
  }

  template<stdexec::receiver R>
  auto connect(R r) {
    return stdexec::connect(std::move(sender), InspectReceiver<R, F> {std::move(r), std::move(func)});
  }

  decltype(auto) get_env() const noexcept { return stdexec::get_env(sender); }

  Child sender;
  F func;
};

struct Inspect {
  template<stdexec::sender S, class F>
  auto operator()(S sender, F func) const {
    return InspectSender<S, F> {.sender = std::move(sender), .func = std::move(func)};
  }

  template<class F>
  auto operator()(F func) const {
    return stdexec::__closure(*this, std::move(func));
  }
};

} // namespace detail

inline constexpr detail::Inspect inspect {};

} // namespace mono::ex

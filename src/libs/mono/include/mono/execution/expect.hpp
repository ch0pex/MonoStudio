#pragma once

#include "mono/meta/concepts.hpp"
#include "stdexec/__detail/__execution_fwd.hpp"

#include <stdexec/execution.hpp>

#include <exception>
#include <expected>
#include <type_traits>
#include <utility>

namespace mono::ex {

namespace detail {

template<class R, class F>
class ExpectReceiver {
public:
  using receiver_concept = stdexec::receiver_t;

  ExpectReceiver(R r, F f) : receiver(std::move(r)), func(std::move(f)) { }

  template<class... As>
    requires(std::invocable<F, As...> and meta::specialization_of<std::invoke_result_t<F, As...>, std::expected>)
  void set_value(As&&... as) noexcept try {
    auto result = std::invoke(func, std::forward<As>(as)...);

    if (not result) {
      throw result.error();
    }

    stdexec::set_value(std::move(receiver), result.value());
  }
  catch (...) {
    stdexec::set_error(std::move(receiver), std::current_exception());
  }

  template<class... As>
    requires(std::invocable<F, As...> and meta::specialization_of<std::invoke_result_t<F, As...>, std::optional>)
  void set_value(As&&... as) noexcept try {
    auto result = std::invoke(func, std::forward<As>(as)...);

    if (not result) {
      throw std::make_exception_ptr("Expected function returned empty optional");
    }

    stdexec::set_value(std::move(receiver), result.value());
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

template<stdexec::sender S, class F>
struct ExpectSender {
  using sender_concept = stdexec::sender_t;

  template<class... Args>
  using set_value_t =
      stdexec::completion_signatures<stdexec::set_value_t(typename std::invoke_result_t<F, Args...>::value_type)>;
  using except_ptr_sig = stdexec::completion_signatures<stdexec::set_error_t(std::exception_ptr)>;

  template<class Env>
  static constexpr auto get_completion_signatures(Env&& env [[maybe_unused]]) noexcept
      -> stdexec::transform_completion_signatures_of<S, Env, except_ptr_sig, set_value_t> //
  {
    return {};
  }

  template<stdexec::receiver R>
  auto connect(R r) {
    return stdexec::connect(std::move(sender), ExpectReceiver<R, F> {std::move(r), std::move(func)});
  }

  decltype(auto) get_env() const noexcept { return stdexec::get_env(sender); }

  S sender;
  F func;
};

struct Expect {
  template<stdexec::sender S, class F>
  auto operator()(S sender, F func) const {
    return ExpectSender<S, F> {.sender = std::move(sender), .func = std::move(func)};
  }

  template<class F>
  auto operator()(F func) const {
    return stdexec::__closure(*this, std::move(func));
  }
};

} // namespace detail


/**
 * Sender adapter that applies a function to the value produced by a sender,
 * expecting the function to return a std::expected or std::optional
 *
 * If the result indicates failure, the error is propagated to the error channel.
 */
inline constexpr detail::Expect expect {};

} // namespace mono::ex

#pragma once

#include "mono/execution/expect.hpp"
#include "mono/execution/inspect.hpp"

#include <exec/single_thread_context.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/timed_thread_scheduler.hpp>
#include <stdexec/execution.hpp>

// clang-format off

namespace mono::ex {

// std P2300
using stdexec::then;
using stdexec::just;
using stdexec::just_error;
using stdexec::run_loop;
using stdexec::when_all;
using stdexec::on;
using stdexec::starts_on;
using stdexec::continues_on;
using stdexec::start_detached; // Will disapear
using stdexec::sync_wait;
using stdexec::sync_wait_with_variant;
using stdexec::let_value;
using stdexec::let_error;
using stdexec::let_stopped;
using stdexec::sender;
using stdexec::receiver;
using stdexec::split;

// exec 
using exec::static_thread_pool;
using exec::timed_thread_scheduler;
using exec::timed_thread_context;
using exec::single_thread_context;

} // namespace mono::ex

// clang-format on


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <chrono>

using namespace std::chrono_literals;


TEST_SUITE_BEGIN("Thread");

TEST_CASE("Basic sender") {
  //   // // Declare a pool of 3 worker threads:
  //
  //   exec::static_thread_pool pool(3);
  //
  //   // Get a handle to the thread pool:
  //   auto sched = pool.get_scheduler();
  //   //
  //   // auto work = stdexec::when_all(
  //   //     stdexec::schedule(sched) | stdexec::then(run_game), //
  //   //     stdexec::schedule(sched) | stdexec::then(run_render), //
  //   //     stdexec::schedule(sched) | stdexec::then(play_sound)
  //   // );
  //   //
  //   // for (std::size_t i = 0; i < 60; ++i) {
  //   //   stdexec::sync_wait(work);
  //   //   // // {
  //   //   // //   std::stringstream ss {};
  //   //   // //   ss << std::this_thread::get_id();
  //   //   // //   std::this_thread::sleep_for(3ms);
  //   //   // //   LOG_INFO("Running game thread: {}", ss.str());
  //   //   // // }
  //   //   // //
  //   //   // // {
  //   //   // //   std::stringstream ss {};
  //   //   // //   ss << std::this_thread::get_id();
  //   //   // //   std::this_thread::sleep_for(5ms);
  //   //   // //   LOG_INFO("Running render thread: {}", ss.str());
  //   //   // // }
  //   //   // //
  //   //   // // {
  //   //   // //   std::stringstream ss {};
  //   //   // //   ss << std::this_thread::get_id();
  //   //   // //   std::this_thread::sleep_for(1ms);
  //   //   // //   LOG_INFO("Running sound thread: {}", ss.str());
  //   //   // // }
  //   // }
  // }
  //
  // stdexec::sender auto async_inclusive_scan(
  //     stdexec::scheduler auto sch, // 2
  //     std::span<double const> input, // 1
  //     std::span<double> output, // 1
  //     double init, // 1
  //     std::size_t tile_count
  // ) // 3
  // {
  //   using namespace stdexec;
  //   std::size_t const tile_size = (input.size() + tile_count - 1) / tile_count;
  //
  //   std::vector<double> partials(tile_count + 1); // 4
  //   partials[0] = init; // 4
  //
  //   return just(std::move(partials)) // 5
  //          | starts_on(sch) |
  //          bulk(
  //              par, tile_count, // 6
  //              [=](std::size_t i, std::vector<double>& partials) { // 7
  //                auto start      = i * tile_size; // 8
  //                auto end        = std::min(input.size(), (i + 1) * tile_size); // 8
  //                partials[i + 1] = *--std::inclusive_scan(
  //                    begin(input) + start, // 9
  //                    begin(input) + end, // 9
  //                    begin(output) + start
  //                ); // 9
  //              }
  //          ) // 10
  //          | then( // 11
  //                [](std::vector<double>&& partials) {
  //                  std::inclusive_scan(
  //                      begin(partials), end(partials), // 12
  //                      begin(partials)
  //                  ); // 12
  //                  return std::move(partials); // 13
  //                }
  //            ) |
  //          bulk(
  //              par, tile_count, // 14
  //              [=](std::size_t i, std::vector<double>& partials) { // 14
  //                auto start = i * tile_size; // 14
  //                auto end   = std::min(input.size(), (i + 1) * tile_size); // 14
  //                std::for_each(
  //                    begin(output) + start, begin(output) + end, // 14
  //                    [&](double& e) { e = partials[i] + e; } // 14
  //                );
  //              }
  //          ) |
  //          then( // 15
  //              [=](std::vector<double>&& partials) { // 15
  //                return output; // 15
  //              }
  //          ); // 15
}

TEST_CASE("Inclusive scan") {

  // exec::static_thread_pool thread_pool(std::thread::hardware_concurrency());
  //
  // auto sched = thread_pool.get_scheduler();
  //
  // auto input = std::views::iota(1, 500'000'000) | std::ranges::to<std::vector<double>>();
  // std::vector<double> output(input.size());
  // stdexec::never_stop_token
  //
  //     stdexec::sync_wait(async_inclusive_scan(sched, input, output, 1, 16));
  // // std::print("{}", output);
}


TEST_SUITE_END();

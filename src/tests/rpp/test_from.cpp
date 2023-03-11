//                  ReactivePlusPlus library
//
//          Copyright Aleksey Loginov 2022 - present.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/victimsnino/ReactivePlusPlus
//

#include "copy_count_tracker.hpp"
#include "mock_observer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <rpp/sources.hpp>

#include <array>



// template<typename T>
// struct oper{};

// template<class T>
// concept IsComplete = requires(T self)
// {
//    {
//       // You can't apply sizeof to an incomplete type
//       sizeof(self)
//    };
// };

// struct Obs
// {
//     template<IsComplete T>
//     typename T::template inner<Obs&> op(const oper<T>* = {})
//     {
//         return {*this};
//     }

//     template<typename T>
//     auto operator[](const oper<T>&)
//     {
//         using TT= typename T::template inner<Obs&>;
//         return TT{*this};
//     }

//     template<typename T>
//     auto operator()()
//     {
//         using TT= typename T::template inner<Obs&>;
//         return TT{*this};
//     }
// };

// namespace operators{

// /**
// * \brief Transform the items emitted by an Observable via applying a function to each item and emitting result
// * \note The Map operator can keep same type of value or change it to some another type.
// * 
// * \marble map
// {
//     source observable       : +--1   -2   --3   -|
//     operator "map: x=>x+10" : +--(11)-(12)--(13)-|
// }
// *
// * \details Actually this operator just applies callable to each obtained emission and emit resulting value
// *
// * \param callable is callable used to provide this transformation. Should accept Type of original observable and return type for new observable
// * \return new specific_observable with the Map operator as most recent operator.
// * \warning #include <rpp/operators/map.hpp>
// * 
// * \par Example with same type:
// * \snippet map.cpp Same type
// *
// * \par Example with changed type:
// * \snippet map.cpp Changed type
// *
// * \par Implementation details:
// * - <b>On subscribe</b>
// *    - None
// * - <b>OnNext</b>
// *    - Just forwards result of applying callable to emissions
// * - <b>OnError</b>
// *    - Just forwards original on_error
// * - <b>OnCompleted</b>
// *    - Just forwards original on_completed 
// *
// * \ingroup transforming_operators
// * \see https://reactivex.io/documentation/operators/map.html
// */
// struct map_t;

// //struct map_t : oper<map_t>
// //{
// //    template<typename TObs>
// //    struct inner
// //    {
// //        TObs obs;
// //
// //        /**
// //         *
// //         * \brief Apply func
// //         *
// //         */
// //        template<typename T>
// //        auto operator()(T v)
// //        {
// //            return obs;
// //        }
// //    };
// //};

// /**
// * \brief Transform the items emitted by an Observable via applying a function to each item and emitting result
// * \note The Map operator can keep same type of value or change it to some another type.
// * 
// * \marble map
// {
//     source observable       : +--1   -2   --3   -|
//     operator "map: x=>x+10" : +--(11)-(12)--(13)-|
// }
// *
// * \details Actually this operator just applies callable to each obtained emission and emit resulting value
// *
// * \param callable is callable used to provide this transformation. Should accept Type of original observable and return type for new observable
// * \return new specific_observable with the Map operator as most recent operator.
// * \warning #include <rpp/operators/map.hpp>
// * 
// * \par Example with same type:
// * \snippet map.cpp Same type
// *
// * \par Example with changed type:
// * \snippet map.cpp Changed type
// *
// * \par Implementation details:
// * - <b>On subscribe</b>
// *    - None
// * - <b>OnNext</b>
// *    - Just forwards result of applying callable to emissions
// * - <b>OnError</b>
// *    - Just forwards original on_error
// * - <b>OnCompleted</b>
// *    - Just forwards original on_completed 
// *
// * \ingroup transforming_operators
// * \see https://reactivex.io/documentation/operators/map.html
// */
// inline constexpr const oper<map_t>* map{};
// struct filter{};
// }



SCENARIO("from iterable", "[source][from]")
{
    // {
    //     Obs obs{};
    //     obs.op(operators::map)();
    // }

    auto mock = mock_observer<int>();
    GIVEN("observable from iterable")
    {
        auto vals = std::vector{1, 2, 3, 4, 5, 6};
        auto obs  = rpp::source::from_iterable(vals);
        WHEN("subscribe on it")
        {
            CHECK(obs.subscribe(mock).is_disposed());
            THEN("observer obtains values in the same order")
            {
                CHECK(mock.get_received_values() == vals);
                CHECK(mock.get_on_completed_count() == 1);
            }
        }
    }
    // GIVEN("observable from iterable with scheduler")
    // {
    //     auto vals     = std::vector{1, 2, 3, 4, 5, 6};
    //     auto run_loop = rpp::schedulers::run_loop{};
    //     auto obs      = rpp::source::from_iterable(vals, run_loop);
    //     WHEN("subscribe on it and dispatch once")
    //     {
    //         obs.subscribe(mock);
    //         run_loop.dispatch();
    //         THEN("observer obtains first value")
    //         {
    //             CHECK(mock.get_received_values() == std::vector{1});
    //             CHECK(mock.get_on_error_count() == 0);
    //             CHECK(mock.get_on_completed_count() == 0);
    //         }
    //         AND_WHEN("dispatch till no events")
    //         {
    //             while (!run_loop.is_empty())
    //                 run_loop.dispatch();
    //
    //             THEN("observer obtains values in the same order")
    //             {
    //                 CHECK(mock.get_received_values() == vals);
    //                 CHECK(mock.get_on_completed_count() == 1);
    //             }
    //         }
    //     }
    //     WHEN("subscribe on it, unsubscribe on first on next and dispatch till not empty")
    //     {
    //         rpp::composite_subscription sub{};
    //         obs.subscribe(sub,
    //                       [&](const auto& v)
    //                       {
    //                           mock.on_next(v);
    //                           sub.unsubscribe();
    //                       });
    //
    //         size_t dispatch_count{};
    //         while (!run_loop.is_empty())
    //         {
    //             ++dispatch_count;
    //             run_loop.dispatch();
    //         }
    //
    //         THEN("observer obtains first value")
    //         {
    //             CHECK(mock.get_received_values() == std::vector{ 1 });
    //             CHECK(mock.get_on_error_count() == 0);
    //             CHECK(mock.get_on_completed_count() == 0);
    //             CHECK(sub.is_subscribed() == false);
    //             CHECK(dispatch_count == 1);
    //         }
    //     }
    // }
    // GIVEN("observable from iterable with exceiption on begin")
    // {
    //     auto run_loop = rpp::schedulers::run_loop{};
    //     auto obs = rpp::source::from_iterable(my_container_with_error{}, run_loop);
    //     WHEN("subscribe on it and dispatch once")
    //     {
    //         obs.subscribe(mock);
    //         run_loop.dispatch();
    //         THEN("observer obtains error")
    //         {
    //             CHECK(mock.get_total_on_next_count() == 0);
    //             CHECK(mock.get_on_error_count() == 1);
    //             CHECK(mock.get_on_completed_count() == 0);
    //         }
    //     }
    // }
}

SCENARIO("from iterable", "[source][from][track_copy]")
{
    copy_count_tracker tracker{};
    auto               vals         = std::array{tracker};
    auto               initial_copy = tracker.get_copy_count();
    auto               initial_move = tracker.get_move_count();

    GIVEN("observable from copied iterable")
    {
        auto obs = rpp::source::from_iterable(vals/*, rpp::schedulers::immediate{}*/);
        WHEN("subscribe on it")
        {
            obs.subscribe();
            THEN("no extra copies")
            {
                CHECK(tracker.get_copy_count() - initial_copy == 1); // 1 copy to wrapped container
                CHECK(tracker.get_move_count() - initial_move <= 2); // 1 move to lambda, 1 move lambda to observable
            }
        }
    }
    GIVEN("observable from moved iterable")
    {
        auto obs = rpp::source::from_iterable(std::move(vals)/*, rpp::schedulers::immediate{}*/);
        WHEN("subscribe on it")
        {
            obs.subscribe();
            THEN("no extra copies")
            {
                CHECK(tracker.get_copy_count() - initial_copy == 0); 
                CHECK(tracker.get_move_count() - initial_move <= 3); // 1 move to wrapped container + 1 move to lambda, 1 move lambda to observable
            }
        }
    }

    GIVEN("observable from copied iterable with shared memory model")
    {
        auto obs = rpp::source::from_iterable<rpp::memory_model::use_shared>(vals);
        WHEN("subscribe on it")
        {
            obs.subscribe();
            THEN("no extra copies")
            {
                CHECK(tracker.get_copy_count() - initial_copy == 1); // 1 copy to shared_ptr
                CHECK(tracker.get_move_count() - initial_move == 0);
            }
        }
    }
    GIVEN("observable from moved iterable")
    {
        auto obs = rpp::source::from_iterable<rpp::memory_model::use_shared>(std::move(vals));
        WHEN("subscribe on it")
        {
            obs.subscribe();
            THEN("no extra copies")
            {
                CHECK(tracker.get_copy_count() - initial_copy == 0);
                CHECK(tracker.get_move_count() - initial_move == 1); // 1 copy to shared_ptr
            }
        }
    }
}
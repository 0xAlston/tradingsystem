cmake_minimum_required(VERSION 3.23)
project(trading_system)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(trading_system
        AlgoExecutionService.hpp
        AlgoStreamingService.hpp
        Connectors.hpp
        DataGenerator.hpp
        ExecutionService.hpp
        GUIService.hpp
        HistoricalDataService.hpp
        InquiryService.hpp
        Listeners.hpp
        main.cpp
        MarketDataService.hpp
        PositionService.hpp
        PricingService.hpp
        Products.hpp
        RiskService.hpp
        SOA.hpp
        StreamingService.hpp
        TradeBookingService.hpp
        )


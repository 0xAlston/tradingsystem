# Final Proj for OOP in Finance 

Xuebin(Alston) Gui

Compile `main.cpp` to run, it takes roughly 10 mins or more, and the streaming process and orderbook data process are the main bottleneck of time consumption.

- The `main.cpp` contains the following steps for testing the bond tradingsystem

  - Step 0: call`Generate_Data()` to initialize the input files, including  `trades`,`prices` ,`marketdata` and `inquiries`

    - trades: product ID trade ID book price quantity side
    - prices: product ID bid-price ask-price
    - market data: product ID first 5 level of bid and ask
    - Inquiries: product ID inquiry ID price quantity side and inquiry state

  - Step 1: create the Service and link them together. Then we process the trades file and generate the position and risk file output.

    ```mermaid
    graph LR
    A[(trades)]-->B(TradingBookService)
    B-->C(Position Service)
    C-->D(RiskService)
    C-->F(HistoricalPositionService)
    D-->E[(risk)]
    F-->G[(pos)]
    ```

  - Step 2: Process the price data file and generate gui-snapshot and streaming file output

    ```mermaid
    graph LR
    A[(prices)]-->B(PricingService)
    B-->C(GUIService)
    C-->D[(gui-snapshot)]
    B-->E(AlgoStreamingService)
    E-->T(StreamingService)
    T-->F(HistoricalStreamingService)
    F--->G[(streaming)]
    ```

  - Step 3: Process the order book file and generate execution, pos and risk file

    ```mermaid
    graph LR
    A[(marketdata)]-->B(MarketDataService)
    B-->C(AlgoexecutionService)
    C-->D(ExecutionServicer)
    D-->E(TradBookingService)
    
    
    E-->H(Position Service)
    H-->i(RiskService)
    H-->k(HistoricalPositionService)
    i-->j[(risk)]
    k-->p[(pos)]
    D-->F(HistoricalExecutionService)
    F-->G[(execution)]
    
    
    ```

  - Step 4: Process the inquiry file and generate output data

    ```mermaid
    graph LR
    A[(inquiries)]-->B(inquiry Service)
    B-->C(HistoricalInquiryService)
    C-->D[(inquiry)]
    ```

    


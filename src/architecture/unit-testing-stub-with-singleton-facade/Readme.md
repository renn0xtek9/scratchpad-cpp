# Unit testing a Stub with a Singleton facade that has mock as dependency injection

## Problem description
In this example we have a header of a [third_party_class](./third_party_class.h) for a third party binary library. We cannot modify it.
We want to implement test where we basically mock the functionality of this binary.
That means we will implement a stub ([third_party_class_stub](./third_party_class_stub.cpp)) that implement against this class but provide a stub implementation.  

In this stub, the [setData](./third_party_header.h#L9) shall write data to a csv file.
We want to test the correct functionality of the stub, by asserting that a call to csv writer class is made when `setData` is called.  
**Problem** because we cannot change [third_party_class](./third_party_class.h), we cannot use simply a dependency injection inside the stub and inject it with a mock of the csv writer inside the [unit test](./tests/stub_unit_test.cpp) of the test agent.


## Approach
We use a [CsvLoggerInterface](./csv_logger_interface.h) that is derived by [CsvLoggerWriter](./csv_logger_writer.h), for the actual usage of the stub (i.e actually writing line to CSV file) and [CsvLoggerMock](./csv_logger_mock.h) used for unit testing the [ThirdPartyClass Stub](./third_party_class_stub.cpp).
The selection of the implementation (actual implementation or mock) is abstracted away by the [CsvLoggerFacadeSingleton](./csv_logger_facade_singleton.h) which by [default](./csv_logger_facade_singleton.h#L11) will have the actual implementation as dependency injection.

## Tests
The Singleton facade itself is [unit tested] in [./tests/csv_logger_facade_singleton_test.cpp]

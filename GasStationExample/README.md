# Gas Station example to illustrate condition varliables in C

- In this example, we have a shared resource `fuel` that is being operated on by 2 threads: `fuel_filling` service and `car_fueling` service.

- We used `condition_variable` mechanism to let the `fuel_filling` service signals to all threads or one thread of `car_fueling` type who are waiting for the facility to have enough fuel, that it is done filling, and they can take over to refuel.
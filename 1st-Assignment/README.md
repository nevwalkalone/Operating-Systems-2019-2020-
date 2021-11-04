# First Assignment

Read this [section](https://github.com/nevwalkalone/POSIX-Projects/blob/main/README.md) (if you haven't already), before proceeding.

The pizzeria has 𝑁𝑐𝑜𝑜𝑘 cooks and 𝑁𝑜𝑣𝑒𝑛 ovens. In the 1st assignment, the pizzeria
does not have a delivery service, i.e. customers can only take their orders out. The first order is
made at the time 0, and every next order is made after a random period of time and it ranges in
the interval [𝑇𝑜𝑟𝑑𝑒𝑟𝑙𝑜𝑤, 𝑇𝑜𝑟𝑑𝑒𝑟ℎ𝑖𝑔ℎ]. Every order includes a random integer number of pizzas in
the interval [𝑁𝑜𝑟𝑑𝑒𝑟𝑙𝑜𝑤, 𝑁𝑜𝑟𝑑𝑒𝑟ℎ𝑖𝑔ℎ]. The order has to wait until a cook is available. When a cook
is available, a 𝑇𝑝𝑟𝑒𝑝 amount of time is required for each pizza to be prepared for the oven. Then,
the cook waits until an oven is available. When an oven is available, all pizzas of the particular
order go into the same oven and bake for 𝑇𝑏𝑎𝑘𝑒 amount of time, while the cook is busy
watching this oven. When the baking is over, the cook puts the completed order on the delivery
bench and is available for the preparation of another order. The cook deals with only one order
each time from the moment the order is handed to him/her until it is placed on the delivery
bench.

## Useful Reads

For a full overview it is highly suggested that you read:

- [Project Assignment](assignment-report/project1-assignment.pdf), that contains all the details about the assignment's tasks.
- [Project Report](assignment-report/project1-report.pdf), that contains all the details about the actual implementation of this system and the analysis of the code.

## Program Execution

1. **Clone** repository and change directory to src.

   ```console
   git clone https://github.com/nevwalkalone/POSIX-Projects.git
   cd 1st-Assignment/src
   ```

2. **Run** [test-res1.sh](src/test-res1.sh) script which will compile and execute your program with 2 parameters, 100 (total number of orders) and 1000 (for the initial seed).

   ```console
   ./test-res1.sh
   ```

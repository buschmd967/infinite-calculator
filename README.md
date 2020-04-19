## Supported actions:

  * Addition (+)
  * Subtraction (-)
    * Note: only supports positive results, will throw an error if it encounters a negative number.
  * Multiplication (\*)
  * Power (^)


##Compilation Instructions:
use `gcc main.c -lm` to compile


## Usage:

  `./my-calc <input file> <output file>`

  The input file must include a line(s) consisting of numbers separated by any symbol listed above. An example of this might be:

  `2 + 2`

  You can also string together multiple operations:

  `2 + 2 - 4`

  If multiple lines are included, multiple answers will be written to the out file separated by one line. If the in file is:

  ```
  2 + 2
  4 * 4
  ```

  The outfile would look like:

 ```
 4
 16
  ```



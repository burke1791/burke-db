# Tuple Structs and API



# Tuple Process Flow

## Inserting a Tuple

- Knowns:
    - table name
    - column names
    - mapping of values to target column names
    - length of variable-length columns

1. Construct a TupleDescriptor by looking up column information in the system tables
    - Includes each column's data type and null/non-null constraint
1. Validate data types against the TupleDescriptor
1. Validate null/non-null status
1. Compute the byte-length of the new tuple
1. Construct a Datum array and isnull array for all values we're inserting
1. Construct a Tuple using the Datum and isnull arrays
1. Insert the tuple into a page with available space

[test]

    control=15

    valueone=1
    valueone=2
    valueone=3
    valueone=4
    valueone=5
    valueone=6
    valueone=7
    valueone=8
    valueone=9

    valuetwo=1
    valuetwo=2
    valuetwo=3
    valuetwo=4
    valuetwo=5
    valuetwo=6
    valuetwo=7
    valuetwo=8
    valuetwo=9
    valuetwo=10
    valuetwo=11
    valuetwo=12
    valuetwo=13
    valuetwo=14

[]

[test]
    bad_two_numbers=6
    bad_two_numbers=7
    bad_real=8.2
    bad_string='some_string'
    [./valuethree]
    [../]
    [./value_bad_one]
    [../]
    [./value_bad_two]
    [../]
    [./value_bad_three]
    [../]
[]

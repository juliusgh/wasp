
[test]
    controlone=15
    controltwo=-45.3
    bad_two_numbers=6
    bad_two_numbers=7
    bad_string='some_string'
    valueone=58.8
    valuetwo=' 65 56 58.8 58 88.7 '
    valuethree=23.9
    valuefour=' 18.3 20.3 23.1 12.34 2e2 '
    valuefive=19
    valuesix=' 11 12 18.2 14 15.1 '
    valueseven=-45.1
    valueeight=' -52.4 -41.9 -100.3 -45.3 -4E-8 -7e+3 '
    [./value_bad_one]
        inside=47
    [../]
    [./value_bad_two]
        inside=48
    [../]
    valueone=a_string
[]

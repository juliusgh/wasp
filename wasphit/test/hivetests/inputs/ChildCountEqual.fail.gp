
[test]

    one=' a b c '
    one=d
    one=' e f '

    two=' 1 2 3 '
    two=4
    two=' 5 6 7 '

    three=' ban att pou '
    three=dol
    three=' per car '

    four=' red orange yellow '
    four=green
    four=' blue indigo '

    six=' algebra chemistry history '
    six=calculus
    six=' physics geometry '

    [./badflags]
    [./]

    [settings]
        [override]
            color=orange
        []
    []
    [settings]
        [override]
        []
        orange_rgb=' 255 165 0 '
    []
    [settings]
        [override]
            color=yellow
        []
        yellow_rgb=' 255 165 0 '
        yellow_rgb=' 250 170 0 '
    []
    [settings]
        [override]
            color=yellow
            color=yellow
            color=yellow
        []
        yellow_rgb=' 255 165 0 '
    []

[]

This test is designed to test whether your game has validated some user input/utilised defensive programming. Your program should not segfault or terminate at any point during the test. The following details why each command is invalid: 

turn 5 L 1 (tile not there)
turn 4 L 1
turn 0 F 2 (can't choose first player tile)
5 U 2 (must have the "turn" command)
turn 3 R 5
turn 6 L 5 (factories can only be from 0 - 5)
turn 5 U 2
turn 0 U 3
turn 2 asd 5 (tile colour should be valid)
turn asd B 5 (factory should be an int)
turn 0 B asd (mosaic row should be an int)
turn 0 B 2 (row is already filled with a different colour)
turn 0 B 3
turn 1 Y 6 (mosaic storage can only be 1 - 5)

After these commands, your game should also be able to save normally and only have saved the outcomes of valid turns. This test does not cover every possible invalid command.


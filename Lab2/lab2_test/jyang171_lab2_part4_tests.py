# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).

##tests = [ {'description': 'This test will run first.',
#    'steps': [ {'inputs': [('PINA',<val>)], 'iterations': 1 } ],
#    'expected': [('PORT',<val>)],
#    },
#    {'description': 'This test will run second.',
#    'steps': [ {'inputs': [('PIN', <val>)],'iterations': 1}, # Set PIN to val then run one iteration
#        {'inputs': [('PIN',<val>)], 'time': 300 }, # Set PIN to val then run 300 ms
#        {'inputs': [('PIN',<val>)], 'iterations': 1, 'expected': [('PORT',<val>)]}, 
#        {'inputs': [('PIN',<val>)], 'time': 600}, ],
#    'expected': [('PORT',<val>)],
#    },
##    ]

# Optionally you can add a set of "watch" variables these need to be global or static and may need
# to be scoped at the function level (for static variables) if there are naming conflicts. The 
# variables listed here will display everytime you hit (and stop at) a breakpoint

#watch = ['<function>::<static-var>','PORTB']

tests = [ {'description': '(50)PINA: 0x32, (60)PINB: 0x3C, (120)PINC: 0x78 => (230)PORTD: 0x39 (111001 | 01)',
    'steps': [ {'inputs': [('PINA',0x32),('PINB',0x3C),('PINC',0x78)], 'iterations': 5 } ],
    'expected': [('PORTD',0x39)],
    },
    {'description': '(22)PINA: 0x16, (90)PINB: 0x5A, (122)PINC: 0x7A => (234)PORTD: 0x3B (111010 | 11)',
    'steps': [ {'inputs': [('PINA',0x16),('PINB',0x5A),('PINC',0x7A)], 'iterations': 5 } ],
    'expected': [('PORTD',0x3B)],
    },
    {'description': '(0)PINA: 0x00, (0)PINB: 0x00, (96)PINC: 0x60 => (96)PORTD: 0x1A (11000 | 10)',
    'steps': [ {'inputs': [('PINA',0x00),('PINB',0x00),('PINC',0x60)], 'iterations': 5 } ],
    'expected': [('PORTD',0x1A)],
    },
    {'description': '(30)PINA: 0x1E, (60)PINB: 0x3C, (40)PINC: 0x28 => (130)PORTD: 0x20 (100000 | 00)',
    'steps': [ {'inputs': [('PINA',0x1E),('PINB',0x3C),('PINC',0x28)], 'iterations': 5 } ],
    'expected': [('PORTD',0x20)],
    },
    {'description': '(64)PINA: 0x40, (64)PINB: 0x40, (64)PINC: 0x40 => (192)PORTD: 0x31 (110000 | 01)',
    'steps': [ {'inputs': [('PINA',0x40),('PINB',0x40),('PINC',0x40)], 'iterations': 5 } ],
    'expected': [('PORTD',0x31)],
    },
    {'description': '(100)PINA: 0x64, (60)PINB: 0x3C, (30)PINC: 0x1E => (190)PORTD: 0x2D (101111 | 01)',
    'steps': [ {'inputs': [('PINA',0x64),('PINB',0x3C),('PINC',0x1E)], 'iterations': 5 } ],
    'expected': [('PORTD',0x2D)],
    },
    ]
#watch = ['PORTD']





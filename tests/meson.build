nomalloc = environment({'MALLOC_PERTURB_': '0'})

bash = find_program('bash')
testTimeout = 240

testSet = [ 
  'lvl01.joypad',
  'lvl02.joypad',
  'lvl01.arkanoid'
]

# Adding tests to the suite
foreach testFile : testSet
  test(testFile,
       bash,
       workdir : meson.current_source_dir(),
       timeout: testTimeout,
       args : [ 'run_test.sh', baseArkBotTester.path(),  quickerArkBotTester.path(), testFile + '.test', testFile + '.sol' ],
       suite : [ 'arkanoid' ])
endforeach

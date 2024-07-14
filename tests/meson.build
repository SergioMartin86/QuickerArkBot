nomalloc = environment({'MALLOC_PERTURB_': '0'})

bash = find_program('bash')
testTimeout = 240

testSet = [ 
  'lvl01',
  'lvl02',
  'lvl03'
]

# Adding tests to the suite
foreach testFile : testSet
  test(testFile,
       quickerArkBotTester,
       workdir : meson.current_source_dir(),
       timeout: testTimeout,
       args : [  testFile + '.test', testFile + '.sol' ],
       suite : [ 'arkanoid' ])
endforeach

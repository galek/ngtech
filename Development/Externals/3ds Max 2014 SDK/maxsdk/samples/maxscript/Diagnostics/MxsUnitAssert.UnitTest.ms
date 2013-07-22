--1
--2
--3
(
	print "Assert Tester"
	AssertReporter.Clear()
	
	-- =========================================================
	-- Test basic true/false tests
	assert_true true
	assert_false ("hh" == "hi")
	assert_false false
	
	val = 5
	assert_equal 5 val message:"These numbers should be the same"
	
	-- =========================================================
	-- Test Equal methods
	h = bitmap 40 40
	assert_equal h h             message:"These bitmaps should be the same"
	
	-- =========================================================
	-- Test Not Equal methods
	assert_not_equal "sdastas" h message:"These are not the same"
	
	-- =========================================================
	-- Test defined functions
	assert_defined   h     message:"These should be defined"
	assert_undefined chris message:"Chris should be undefined"
	
	-- =========================================================
	-- Test Float comparision functions
	assert_float 5.6 5.6
	local val = 9.8925632456
	assert_not_equal 5.6 val 
	
	-- =========================================================
	-- Test String comparison
	-- Default is to ignore case
	assert_string_equal "chris" "chris"
	assert_not_equal "chris" "Joe" 
	assert_string_equal "jack" "jack" message:"This should fail" ignorecase:false
	assert_string_equal "Hannah" "HANNAH"
	
	-- =========================================================
	-- Test Point3 comparison
	assert_point3_equal [0,0,0] [0,0,0]
	assert_not_equal [0,0,0] [4,0,0]
	assert_not_equal [0,0,0] [0,5,0]
	assert_not_equal [0,0,0] [0,0,6] 
	assert_point3_equal [4.8,0,0] [4.9,0,0] tolerance: 0.2 -- This should pass
	assert_not_equal [4.8,0,0] [4.9,0,0] 
	
	-- =========================================================
	-- Test matrix comparison
	local mat1 = (matrixFromNormal [15,15,60])
	assert_matrix_equal (matrixFromNormal [15,15,60]) mat1
	assert_not_equal (matrixFromNormal [15,15,55]) mat1
	
	-- =========================================================
	AssertReporter.LogMessage "End of the tests"
	print (AssertReporter.GetMessages())
	assert ((AssertReporter.GetAssertFailures()).count == 0) -- use the old style assert.
	print "All done..."
	ok
)
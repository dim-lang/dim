var x = 1 + 6 / (-3) * (4 + 3 % -2);
var y = 0;
var z = x + y;
var a9 = 228;
var a18 = 123.901;

func Hello(x, y) {
    var a = 1;
    var b = 2;
    a = a + b;
    return a;

    if (a < b)
        print(x);
    else
        print(y);

    while (a < b)
        print(x+y);
}

// this is a line comment */ is for testing block comment ending

/**
 * This is a block comment
 * Which contains a string "hello world"
 * integer: 10243+e31
 */

func ok(y) { var x = 1; return x%y; }

func testOnTokenLexer() {
    print("This is going to be a very long string, which is used for testing programming language");
    print("Since our language defines a string between double quotes, with new line break like this\n and this \r\n.
Let's see if it works as we expect.
Maybe we need more troubleshooting while testing.");
    print(
            "Try punctuations like if \"'\" will stop this string literals."
            "join string 2, "
            "join string 3, "
            "join string 4, "
            );
}
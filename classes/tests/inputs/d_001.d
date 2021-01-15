/+
void brokenFunc() {
    int x=123;  /* the outer block comment does NOT break here */
    int y=321;  /+ Yes, these comments really do nest!
    /+ (even if your syntax highlighter fails to handle them properly!) +/
    +/
}
+/
void workingFunc() {
    
}

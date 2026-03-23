class A {
    public:
    fn void A() {}
    fn int32 test() { return 42; }
}
fn int32 main() {
    dec ptr a = new A();
    if (a.test() == 42) {
        return 1;
    }
    return 0;
}

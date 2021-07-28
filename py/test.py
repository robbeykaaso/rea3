from rea import pipelines

def test0():
    pipelines().add(lambda aInput:
        print(aInput.data()) 
    , {"name": "test"})
    pipelines().add(lambda aInput:
        print(aInput.data()) 
    , {"name": "test2"})
    pipelines().run("test", "hello")
    pipelines().run("test", False)

pipelines().add(lambda aInput:
    print(aInput.data())
, {"name": "testSuccessPy"})

def test1():
    def p1(aInput):
        assert aInput.data() == 3
        aInput.out()
    def p2(aInput):
        assert aInput.data() == 3
        aInput.outs("Pass: test1", "testSuccessPy")
    pipelines() \
    .add(p1, {"name": "test1"}) \
    .nextF(p2)

    pipelines().run("test1", 3)

def test2():
    def p1(aInput):
        assert aInput.data() == 4
        aInput.outs(5, "test2_")
    def p2(aInput):
        assert aInput.data() == 5
        aInput.outs("Pass: test2", "testSuccessPy")
    pipelines() \
    .add(p1, {"name": "test2"}) \
    .nextF(p2, "", {"name": "test2_"})

    pipelines().run("test2", 4)

def test3():
    def p1(aInput):
        assert(aInput.data() == 66)
        aInput.outs("test3", "test3_0")
    pipelines().add(p1, {"name": "test3"}) \
    .next("test3_0") \
    .next("testSuccessPy")

    pipelines().add(lambda aInput:
        aInput.out()
    , {"name": "test3_1"}) \
    .next("test3__") \
    .next("testSuccessPy")

    pipelines().find("test3_0") \
    .nextF(lambda aInput:
        aInput.out()
    , "", {"name": "test3__"}) \
    .next("testSuccessPy")

    def p2(aInput):
        assert aInput.data() == "test3"
        aInput.outs("Pass: test3", "testSuccessJS") 
        aInput.outs("Pass: test3_", "test3__")
    pipelines().add(p2, {"name": "test3_0"})

    pipelines().run("test3", 66)
    pipelines().run("test3_1", "Pass: test3__")
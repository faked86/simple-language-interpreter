#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

class Expression {
public:
    virtual Expression *eval(std::unordered_map<std::string, Expression *> env) = 0;
    virtual int getI() = 0;
    virtual std::string getId() = 0;
    virtual void print(std::ofstream &output) = 0;
    virtual Expression *getBody() = 0;
    virtual std::unordered_map<std::string, Expression *> getOwnEvn() = 0;
    virtual void setOwnEvn(std::unordered_map<std::string, Expression *> env) = 0;
};

Expression *fromEnv(std::string id, std::unordered_map<std::string, Expression *> env) {
    auto it = env.find(id);
    if (it != env.end()) {
        return it->second;
    } else {
        throw "no such id in env";
    }
}

int getValue(Expression *e) {
    // if (typeid(e) != typeid(val)) throw "trying to call getValue from non-val class";
    return e->getI();
}

class val : public Expression {
    int i;

public:
    val(int value) { i = value; }

    Expression *eval(std::unordered_map<std::string, Expression *> env) { return this; }

    int getI() { return i; }
    std::string getId() { throw "val is bad class to call getId"; }
    Expression *getBody() { throw "val is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "val is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) {
        output << "(val " << i << ")";
    }
};

class var : public Expression {
    std::string id;

public:
    Expression *eval(std::unordered_map<std::string, Expression *> env) { return fromEnv(id, env); }

    var(std::string i)
        : id(i) {
    }

    int getI() { throw "var is bad class to call getI"; }
    std::string getId() { throw "var is bad class to call getId"; }
    Expression *getBody() { throw "var is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "var is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) { output << "(var " << id << ")"; }
};

class add : public Expression {
    Expression *e1;
    Expression *e2;

public:
    Expression *eval(std::unordered_map<std::string, Expression *> env) {
        return new val(getValue(e1->eval(env)) + getValue(e2->eval(env)));
    }

    add(Expression *first, Expression *second)
        : e1(first), e2(second) {
    }

    ~add() {
        delete e1;
        delete e2;
    }

    int getI() { throw "add is bad class to call getI"; }
    std::string getId() { throw "add is bad class to call getId"; }
    Expression *getBody() { throw "add is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "add is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) {
        output << "(add ";
        e1->print(output);
        output << " ";
        e2->print(output);
        output << ")";
    }
};

class subtraction : public Expression {
    Expression *e1;
    Expression *e2;

public:
    Expression *eval(std::unordered_map<std::string, Expression *> env) {
        return new val(getValue(e1->eval(env)) - getValue(e2->eval(env)));
    }

    subtraction(Expression *first, Expression *second)
        : e1(first), e2(second) {
    }

    ~subtraction() {
        delete e1;
        delete e2;
    }

    int getI() { throw "add is bad class to call getI"; }
    std::string getId() { throw "add is bad class to call getId"; }
    Expression *getBody() { throw "add is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "add is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) {
        output << "(sub ";
        e1->print(output);
        output << " ";
        e2->print(output);
        output << ")";
    }
};

class ifexpr : public Expression {
    Expression *e1;
    Expression *e2;
    Expression *e_then;
    Expression *e_else;

public:
    Expression *eval(std::unordered_map<std::string, Expression *> env) {
        if (getValue(e1->eval(env)) > getValue(e2->eval(env))) {
            return e_then->eval(env);
        } else {
            return e_else->eval(env);
        }
    }

    int getI() { throw "ifexpr is bad class to call getI"; }
    std::string getId() { throw "ifexpr is bad class to call getId"; }
    Expression *getBody() { throw "ifexpr is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "ifexpr is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) {
        output << "(if ";
        e1->print(output);
        output << " ";
        e2->print(output);
        output << " then ";
        e_then->print(output);
        output << " else ";
        e_else->print(output);
        output << ")";
    }

    ifexpr(Expression *expr1, Expression *expr2, Expression *expr_then, Expression *expr_else)
        : e1(expr1), e2(expr2), e_then(expr_then), e_else(expr_else) {
    }
    ~ifexpr() {
        delete e1;
        delete e2;
        delete e_then;
        delete e_else;
    }
};

class let : public Expression {
    Expression *body;
    std::string id;
    Expression *value;

public:
    Expression *eval(std::unordered_map<std::string, Expression *> env) {
        value->setOwnEvn(env);
        auto pr = env.insert(std::make_pair(id, value));
        if (pr.second == false)
            (pr.first)->second = value;

        return body->eval(env);
    }

    let(std::string i, Expression *val, Expression *b)
        : body(b), value(val), id(i) {
    }
    ~let() {
        delete body;
        delete value;
    }

    int getI() { throw "let is bad class to call getI"; }
    std::string getId() { throw "let is bad class to call getId"; }
    Expression *getBody() { throw "let is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "let is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) {
        output << "(let " << id << " = ";
        value->print(output);
        output << " in ";
        body->print(output);
        output << ")";
    }
};

class function : public Expression {
    std::string id;
    Expression *body;
    std::unordered_map<std::string, Expression *> ownEnv;
    // friend class call;
public:
    function(std::string i, Expression *expression)
        : id(i), body(expression) {
        /*auto it = env.find(id);
        if (it == env.end()) {
                return;
        }
        else {
                throw "function argument name is already taken";
        }*/
    }
    ~function() { delete body; }

    Expression *eval(std::unordered_map<std::string, Expression *> env) { return this; }

    int getI() { throw "function is bad class to call getI"; }
    std::string getId() { return id; }
    Expression *getBody() { return body; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { return ownEnv; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { ownEnv = env; }

    void print(std::ofstream &output) {
        output << "(function " << id << ' ';
        body->print(output);
        output << ")";
    }
};

class call : public Expression {
    Expression *f_expr;
    Expression *arg_expr;

public:
    call(Expression *fExpression, Expression *argExpression)
        : f_expr(fExpression), arg_expr(argExpression) {
    }
    ~call() {
        delete f_expr;
        delete arg_expr;
    }

    Expression *eval(std::unordered_map<std::string, Expression *> env) {
        // if (typeid(function) == typeid(f_expr)) throw "trying to call non function expr";
        // std::cout << "another func call\n";
        Expression *func = f_expr->eval(env);

        std::unordered_map<std::string, Expression *> localEnv = func->getOwnEvn();

        auto pr = localEnv.insert(std::make_pair(func->getId(), arg_expr->eval(env)));
        if (pr.second == false)
            (pr.first)->second = arg_expr->eval(env);

        return func->getBody()->eval(localEnv);
    }

    int getI() { throw "call is bad class to call getI"; }
    std::string getId() { throw "call is bad class to call getId"; }
    Expression *getBody() { throw "call is bad class to call getBody"; }
    std::unordered_map<std::string, Expression *> getOwnEvn() { throw "call is bad class to call getOwnEvn"; }

    void setOwnEvn(std::unordered_map<std::string, Expression *> env) { return; }

    void print(std::ofstream &output) {
        output << "(call ";
        f_expr->print(output);
        output << ' ';
        arg_expr->print(output);
        output << ")";
    }
};

/*class set : public Expression {
        std::string id;
        Expression* value;
public:
        Expression* eval(std::unordered_map<std::string, Expression*> env) {
                auto it = env.find(id);
                if (it != env.end()) {
                        delete it->second;
                        it->second = value;
                }
                else {
                        throw "can't set var because no such id in env";
                }
                return value->eval(env);
        }

        set(std::string i, Expression* val) : value(val), id(i) { }
        ~set() { delete value; }

        int getI() { throw "set is bad class to call getI"; }
        std::string getId() { throw "set is bad class to call getId"; }
        Expression* getBody() { throw "set is bad class to call getBody"; }

        void print(std::ofstream& output) {
                output << "(set " << id << " = ";
                value->print(output);
                output << ")";
        }
};*/

Expression *scan(std::ifstream &input) {
    std::string type;

    char c;
    input.get(c);
    while (c != '(') {
        input.get(c);
    }

    input >> type;

    if (type == "val") {
        int i;
        input >> i;
        return new val(i);
    }
    if (type == "var") {
        input.get();

        std::string id;
        char c;
        input.get(c);
        while (c != ')') {
            id.append(1, c);
            input.get(c);
        }
        return new var(id);
    }
    if (type == "add") {
        Expression *e1 = scan(input);
        Expression *e2 = scan(input);
        return new add(e1, e2);
    }
    if (type == "sub") {
        Expression *e1 = scan(input);
        Expression *e2 = scan(input);
        return new subtraction(e1, e2);
    }
    if (type == "if") {
        Expression *e1 = scan(input);
        Expression *e2 = scan(input);
        Expression *et = scan(input);
        Expression *ee = scan(input);
        return new ifexpr(e1, e2, et, ee);
    }
    if (type == "let") {
        input.get();

        std::string id;
        char c;
        input.get(c);
        while (c != ' ') {
            id.append(1, c);
            input.get(c);
        }
        Expression *value = scan(input);
        Expression *body = scan(input);
        return new let(id, value, body);
    }
    if (type == "function") {
        input.get();

        std::string id;
        char c;
        input.get(c);
        while (c != ' ') {
            id.append(1, c);
            input.get(c);
        }

        Expression *body = scan(input);
        return new function(id, body);
    }
    if (type == "call") {
        Expression *e1 = scan(input);
        Expression *e2 = scan(input);
        return new call(e1, e2);
    }
    /*if (type == "set") {
            input.get();

            std::string id;
            char c;
            input.get(c);
            while (c != ' ') {
                    id.append(1, c);
                    input.get(c);
            }
            Expression* value = scan(input);
            return new set(id, value);
    }*/
    throw "No such expression";
};

int main() {
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    Expression *expr = nullptr;
    Expression *eValueted = nullptr;

    std::unordered_map<std::string, Expression *> env;

    try {
        expr = scan(input);
        eValueted = expr->eval(env);
        eValueted->print(output);
    } catch (const char *error) {
        std::cerr << error;
        output << "ERROR";
    }

    delete expr;
    delete eValueted;
    input.close();
    output.close();
    return 0;
}
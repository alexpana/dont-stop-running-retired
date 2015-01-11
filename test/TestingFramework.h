#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include <cmath>
#include <functional>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <bits/stl_bvector.h>

#include "Vec2.h"
#include "Log.h"

engine::Log _log{"Test"};

namespace test {
    static const double ARITHMETIC_ERROR_THRESHOLD = 0.0000001;

    class Test {
    public:
        virtual ~Test() {
        }

        virtual void beforeClass() {
        }

        virtual void afterClass() {
        }

        virtual void beforeMethod() {
        }

        virtual void afterMethod() {
        }

        virtual void registerTests() {
        }

#define REGISTER_NAMED_TEST(test, name) \
            tests[name] = [&]() -> bool { return test(); }

#define REGISTER_TEST(test) REGISTER_NAMED_TEST(test, #test)

        std::unordered_map<std::string, std::function<bool(void)>> tests;
    };

    class Assert {
    public:
        template<typename T>
        static bool __assertEqualsImpl(T value, T expected, std::string valueStr, std::string expectedStr, std::string sourceFile, int lineNumber) {
            if (value != expected) {
                _log.error() << sourceFile << ":" << lineNumber << " assertion " << valueStr << " == " << expectedStr << " failed." << std::endl;
                return false;
            }

            return true;
        }

        template<typename T>
        static bool __assertFloatEqualsImpl(T value, T expected, std::string valueStr, std::string expectedStr, std::string sourceFile, int lineNumber) {
            if (std::fabs(value - expected) > ARITHMETIC_ERROR_THRESHOLD) {
                _log.error() << sourceFile << ":" << lineNumber << " assertion " << valueStr << " == " << expectedStr << " failed." << std::endl;
                return false;
            }

            return true;
        }

        template<typename T>
        static bool __assertTrueImpl(T expression, std::string expressionStr, std::string sourceFile, int lineNumber) {
            if (!expression) {
                _log.error() << sourceFile << ":" << lineNumber << " assertion \"" << expressionStr << "\" is true failed." << std::endl;
                return false;
            }

            return true;
        }

        template<typename T>
        static bool __assertFalseImpl(T expression, std::string expressionStr, std::string sourceFile, int lineNumber) {
            if (expression) {
                _log.error() << sourceFile << ":" << lineNumber << " assertion \"" << expressionStr << "\" is false failed." << std::endl;
                return false;
            }

            return true;
        }
    };

    struct TestRunReport {
        std::string testCaseName;
        int testsRan = 0;
        int testsFailed = 0;
        int testsCount = 0;
        std::vector<std::pair<std::string, bool>> testResults;
    };

    class TestRunner {
    public:
        void registerTestClass(std::unique_ptr<Test> &&test, std::string name) {
            testClasses[name] = std::move(test);
        }

        void run() {
            for (auto &testClass : testClasses) {
                Test *currentTestClass = testClass.second.get();

                TestRunReport currentTestClassReport;
                currentTestClassReport.testCaseName = testClass.first;

                currentTestClass->registerTests();

                currentTestClass->beforeClass();

                for (auto testMethod : currentTestClass->tests) {
                    _log.info() << "Running test method " << testClass.first << "::" << testMethod.first << std::endl;

                    currentTestClass->beforeMethod();

                    bool testPassed = testMethod.second();

                    currentTestClass->afterMethod();

                    currentTestClassReport.testsRan += 1;
                    currentTestClassReport.testsCount += 1;
                    if (!testPassed) {
                        currentTestClassReport.testsFailed += 1;
                    }

                    currentTestClassReport.testResults.push_back(std::make_pair(testMethod.first, testPassed));

                    _log.info() << std::endl;
                }

                testRunReports.push_back(currentTestClassReport);

                currentTestClass->afterClass();
            }

            _log.info() << std::string(60, '-') << "\n";
            _log.info() << "Test results" << "\n";
            _log.info() << std::string(60, '-') << "\n";
            _log.info() << std::endl;

            for (auto testRunReport : testRunReports) {
                printReport(testRunReport);
            }
        }

        void printReport(const TestRunReport &report) {
            _log.info() << report.testCaseName << "(failed: " << report.testsFailed << ", total: " << report.testsCount << ")" << std::endl;

            for (auto testResult : report.testResults) {
                _log.info() << "    " << testResult.first << std::string(60 - 4 - 8 - testResult.first.length(), '.') << (testResult.second ? "[PASSED]" : "[FAILED]") << std::endl;
            }

            _log.info() << "\n";
        }

    private:

        std::unordered_map<std::string, std::unique_ptr<Test>> testClasses;
        std::vector<TestRunReport> testRunReports;
    };


#define BEGIN_TEST(name)                                                                        \
    bool name() {                                                                               \
        bool testPassed = true;


#define END_TEST                                                                                \
        return testPassed;                                                                      \
    }


#define ASSERT_EQUALS(x, y)                                                                     \
    testPassed &= test::Assert::__assertEqualsImpl(x, y, #x, #y, __FILE__, __LINE__)


#define ASSERT_FLOAT_EQUALS(x, y)                                                               \
    testPassed &= test::Assert::__assertFloatEqualsImpl(x, y, #x, #y, __FILE__, __LINE__)


#define ASSERT_TRUE(x)                                                                          \
    testPassed &= test::Assert::__assertTrueImpl(x, #x, __FILE__, __LINE__)


#define ASSERT_FALSE(x)                                                                         \
    testPassed &= test::Assert::__assertFalseImpl(x, #x, __FILE__, __LINE__)


}

#pragma clang diagnostic pop
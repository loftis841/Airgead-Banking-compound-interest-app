#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

/*
 * Airgead Banking Investment Calculator
 * CS-210 Project Two
 *
 * Key behaviors:
 * - Monthly compounding
 * - Interest rate is annual percent (e.g., 3.5 means 3.5%)
 * - With deposits: this month's deposit does NOT earn interest until next month
 * - Prints yearly details using printDetails() formatting requirements
 */

class InvestmentCalculator {
public:
    // Prints one row of the report with two tabs between fields and 2 decimals.
    static void printDetails(int year, double yearEndBalance, double interestEarned) {
        cout << year << "\t\t"
            << fixed << setprecision(2)
            << "$" << yearEndBalance << "\t\t"
            << "$" << interestEarned << '\n';
    }

    // Calculates balances with NO monthly deposits and prints each year's details.
    static double calculateBalanceWithoutMonthlyDeposit(double initialInvestment,
        double annualInterestRatePercent,
        int numberOfYears) {
        double balance = initialInvestment;
        const double monthlyRate = (annualInterestRatePercent / 100.0) / 12.0;

        for (int year = 1; year <= numberOfYears; ++year) {
            double interestEarnedThisYear = 0.0;

            for (int month = 1; month <= 12; ++month) {
                // Earn interest on current balance (monthly compounding).
                const double monthlyInterest = balance * monthlyRate;
                balance += monthlyInterest;
                interestEarnedThisYear += monthlyInterest;
            }

            // Print yearly summary row.
            printDetails(year, balance, interestEarnedThisYear);
        }

        return balance;
    }

    // Calculates balances WITH monthly deposits and prints each year's details.
    // Deposit timing rule: deposit is added AFTER interest, so it earns starting next month.
    static double balanceWithMonthlyDeposit(double initialInvestment,
        double monthlyDeposit,
        double annualInterestRatePercent,
        int numberOfYears) {
        double balance = initialInvestment;
        const double monthlyRate = (annualInterestRatePercent / 100.0) / 12.0;

        for (int year = 1; year <= numberOfYears; ++year) {
            double interestEarnedThisYear = 0.0;

            for (int month = 1; month <= 12; ++month) {
                // Earn interest on previous balance first.
                const double monthlyInterest = balance * monthlyRate;
                balance += monthlyInterest;
                interestEarnedThisYear += monthlyInterest;

                // Add this month's deposit AFTER interest (does not earn until next month).
                balance += monthlyDeposit;
            }

            // Print yearly summary row.
            printDetails(year, balance, interestEarnedThisYear);
        }

        return balance;
    }
};

// Secure input helper: reads a non-negative double with validation.
double readNonNegativeDouble(const string& prompt) {
    double value = 0.0;

    while (true) {
        cout << prompt;
        cin >> value;

        // Check for stream failure (non-numeric input).
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        // Reject negative numbers for this project.
        if (value < 0.0) {
            cout << "Invalid input. Please enter a value greater than or equal to 0.\n";
            continue;
        }

        // Consume trailing input on the line.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

// Secure input helper: reads a positive int with validation.
int readPositiveInt(const string& prompt) {
    int value = 0;

    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a whole number.\n";
            continue;
        }

        if (value <= 0) {
            cout << "Invalid input. Please enter a value greater than 0.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

void printInputSummary(double initialInvestment,
    double monthlyDeposit,
    double annualInterestRatePercent,
    int numberOfYears) {
    cout << "\n**********************************\n";
    cout << "******** Airgead Banking *********\n";
    cout << "**********************************\n";
    cout << fixed << setprecision(2);
    cout << "Initial Investment Amount: $" << initialInvestment << '\n';
    cout << "Monthly Deposit:           $" << monthlyDeposit << '\n';
    cout << "Annual Interest:           " << annualInterestRatePercent << "%\n";
    cout << "Number of Years:           " << numberOfYears << "\n\n";
}

void printReportHeader(const string& title) {
    cout << title << '\n';
    cout << "Year\t\tYear End Balance\tInterest Earned\n";
    cout << "--------------------------------------------------------\n";
}

int main() {
    char runAgain = 'y';

    while (runAgain == 'y' || runAgain == 'Y') {
        // Collect validated inputs.
        const double initialInvestment = readNonNegativeDouble("Initial Investment Amount: ");
        const double monthlyDeposit = readNonNegativeDouble("Monthly Deposit: ");
        const double annualInterestRatePercent = readNonNegativeDouble("Annual Interest (%): ");
        const int numberOfYears = readPositiveInt("Number of Years: ");

        // Display summary and pause.
        printInputSummary(initialInvestment, monthlyDeposit, annualInterestRatePercent, numberOfYears);
        cout << "Press Enter to continue...";
        cin.get(); // waits for Enter

        // Report 1: without monthly deposits
        cout << "\n";
        printReportHeader("Balance and Interest Without Additional Monthly Deposits");
        InvestmentCalculator::calculateBalanceWithoutMonthlyDeposit(
            initialInvestment, annualInterestRatePercent, numberOfYears
        );

        // Report 2: with monthly deposits
        cout << "\n";
        printReportHeader("Balance and Interest With Additional Monthly Deposits");
        InvestmentCalculator::balanceWithMonthlyDeposit(
            initialInvestment, monthlyDeposit, annualInterestRatePercent, numberOfYears
        );

        // Ask user to run again.
        cout << "\nRun another calculation? (Y/N): ";
        cin >> runAgain;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n";
    }

    cout << "Thank you for using Airgead Banking!\n";
    return 0;
}


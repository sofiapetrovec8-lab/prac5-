#include <iostream>
using namespace std;

const int MAX_N = 10000;

struct Stats {
    long long comparisons = 0;
    long long swaps = 0;
    long long calls = 0;
    int maxDepth = 0;
};

// Копіювання масиву
void copyArray(const int source[], int destination[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}

// Виведення масиву
void printArray(const int a[], int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

// Виведення частини масиву
void printSegment(const int a[], int lo, int hi) {
    for (int i = lo; i <= hi; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

// Обмін двох елементів
void exchange(int a[], int i, int j, Stats& s) {
    if (i != j) {
        int temp = a[i];
        a[i] = a[j];
        a[j] = temp;
        s.swaps++;
    }
}

// Розбиття Ломуто
int partitionLomuto(int a[], int lo, int hi, Stats& s) {
    int pivot = a[hi];
    int i = lo;

    for (int j = lo; j < hi; j++) {
        s.comparisons++;

        if (a[j] <= pivot) {
            exchange(a, i, j, s);
            i++;
        }
    }

    exchange(a, i, hi, s);

    return i;
}

// QuickSort Ломуто
// Оптимізовано, щоб уникнути надмірної рекурсії
void quickSortLomuto(int a[], int lo, int hi, Stats& s,
    int depth = 1, bool trace = false) {
    while (lo < hi) {
        s.calls++;

        if (depth > s.maxDepth) {
            s.maxDepth = depth;
        }

        int p = partitionLomuto(a, lo, hi, s);

        if (trace) {
            cout << "Lomuto: depth=" << depth
                << ", range=[" << lo << "," << hi << "]"
                << ", pivot=" << a[p]
                << ", pivotIndex=" << p
                << ", segment: ";

            printSegment(a, lo, hi);
        }

        // Рекурсивно сортуємо меншу частину,а більшу обробляємо циклом.
        if (p - lo < hi - p) {
            quickSortLomuto(a, lo, p - 1, s,
                depth + 1, trace);

            lo = p + 1;
        }
        else {
            quickSortLomuto(a, p + 1, hi, s,
                depth + 1, trace);

            hi = p - 1;
        }

        depth++;

        if (depth > s.maxDepth) {
            s.maxDepth = depth;
        }
    }
}

// Розбиття Гоара
int partitionHoare(int a[], int lo, int hi, Stats& s) {
    int pivot = a[lo + (hi - lo) / 2];

    int i = lo - 1;
    int j = hi + 1;

    while (true) {
        do {
            i++;
            s.comparisons++;
        } while (a[i] < pivot);

        do {
            j--;
            s.comparisons++;
        } while (a[j] > pivot);

        if (i >= j) {
            return j;
        }

        exchange(a, i, j, s);
    }
}

// QuickSort Гоара
void quickSortHoare(int a[], int lo, int hi, Stats& s,
    int depth = 1, bool trace = false) {
    if (lo >= hi) {
        return;
    }

    s.calls++;

    if (depth > s.maxDepth) {
        s.maxDepth = depth;
    }

    int p = partitionHoare(a, lo, hi, s);

    if (trace) {
        cout << "Hoare: depth=" << depth
            << ", range=[" << lo << "," << hi << "]"
            << ", boundary=" << p
            << ", segment: ";

        printSegment(a, lo, hi);
    }

    quickSortHoare(a, lo, p, s, depth + 1, trace);
    quickSortHoare(a, p + 1, hi, s, depth + 1, trace);
}

// Перевірка сортування
bool isSorted(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            return false;
        }
    }

    return true;
}

// Сума елементів
long long sumArray(const int a[], int n) {
    long long sum = 0;

    for (int i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum;
}

// Сума квадратів
long long sumSquares(const int a[], int n) {
    long long sum = 0;

    for (int i = 0; i < n; i++) {
        sum += 1LL * a[i] * a[i];
    }

    return sum;
}

// Перевірка збереження елементів
bool contentsPreserved(const int before[],
    const int after[], int n) {
    return sumArray(before, n) == sumArray(after, n)
        && sumSquares(before, n) == sumSquares(after, n);
}

// Тест сортування
void sortTest(const int source[], int n, const char* name,
    bool lomuto, int& passed, int& total) {
    int a[MAX_N];

    copyArray(source, a, n);

    Stats s;

    if (lomuto) {
        quickSortLomuto(a, 0, n - 1, s);
    }
    else {
        quickSortHoare(a, 0, n - 1, s);
    }

    bool ok = isSorted(a, n)
        && contentsPreserved(source, a, n);

    cout << (lomuto ? "Lomuto" : "Hoare")
        << " | " << name
        << " | " << (ok ? "OK" : "ERROR") << endl;

    total++;

    if (ok) {
        passed++;
    }
}

// Тест розбиття Ломуто
void partitionTestLomuto(const int source[], int n,
    const char* name, int& passed) {
    int a[MAX_N];
    copyArray(source, a, n);

    Stats s;

    if (n == 0) {
        cout << "Lomuto partition | "
            << name << " | OK" << endl;
        passed++;
        return;
    }

    int p = partitionLomuto(a, 0, n - 1, s);

    bool ok = p >= 0 && p < n;

    for (int i = 0; i < p; i++) {
        if (a[i] > a[p]) {
            ok = false;
        }
    }

    for (int i = p + 1; i < n; i++) {
        if (a[i] <= a[p]) {
            ok = false;
        }
    }

    cout << "Lomuto partition | " << name
        << " | " << (ok ? "OK" : "ERROR") << endl;

    if (ok) {
        passed++;
    }
}

// Тест розбиття Гоара
void partitionTestHoare(const int source[], int n,
    const char* name, int& passed) {
    int a[MAX_N];
    copyArray(source, a, n);

    Stats s;

    if (n < 2) {
        cout << "Hoare partition | "
            << name << " | OK" << endl;
        passed++;
        return;
    }

    int p = partitionHoare(a, 0, n - 1, s);

    bool ok = p >= 0 && p < n - 1;

    for (int i = 0; i <= p; i++) {
        for (int j = p + 1; j < n; j++) {
            if (a[i] > a[j]) {
                ok = false;
            }
        }
    }

    cout << "Hoare partition | " << name
        << " | " << (ok ? "OK" : "ERROR") << endl;

    if (ok) {
        passed++;
    }
}

// Автоматичні тести
void tests() {
    cout << "=== AUTOMATED SORT TESTS ===" << endl;

    const int a1[] = { 7 };
    const int a2[] = { 1, 2 };
    const int a3[] = { 2, 1 };
    const int a4[] = { 1, 2, 3, 4, 5 };
    const int a5[] = { 5, 4, 3, 2, 1 };
    const int a6[] = { 4, 1, 5, 2, 3 };
    const int a7[] = { 3, 1, 3, 2, 1 };
    const int a8[] = { 0, -5, 8, -2, 4 };
    const int a9[] = { 6, 6, 6, 6 };
    const int a10[] = { 9, -1, 9, 0, -1, 5 };
    const int a11[] = { 3, 2, 1, 4, 5 };
    const int a12[] = { 10, 0, -10, 5 };
    const int a13[] = { 2, 2, 1, 3, 1, 2 };

    const int* arrays[] = {
        nullptr, a1, a2, a3, a4, a5, a6,
        a7, a8, a9, a10, a11, a12, a13
    };

    const int sizes[] = {
        0, 1, 2, 2, 5, 5, 5,
        5, 5, 4, 6, 5, 4, 6
    };

    const char* names[] = {
        "empty", "one element", "two sorted",
        "two reversed", "ascending", "descending",
        "mixed", "duplicates", "negative values",
        "all equal", "combined", "partly sorted",
        "own test 1", "own test 2"
    };

    int passed = 0;
    int total = 0;

    for (int i = 0; i < 14; i++) {
        sortTest(arrays[i], sizes[i], names[i],
            true, passed, total);

        sortTest(arrays[i], sizes[i], names[i],
            false, passed, total);
    }

    cout << "Sort tests: " << passed
        << "/" << total << endl;

    cout << "\n=== PARTITION TESTS ===" << endl;

    int pPassed = 0;

    partitionTestLomuto(a3, 2, "two reversed", pPassed);
    partitionTestLomuto(a4, 5, "ascending", pPassed);
    partitionTestLomuto(a5, 5, "descending", pPassed);
    partitionTestLomuto(a9, 4, "all equal", pPassed);

    partitionTestHoare(a3, 2, "two reversed", pPassed);
    partitionTestHoare(a4, 5, "ascending", pPassed);
    partitionTestHoare(a5, 5, "descending", pPassed);
    partitionTestHoare(a9, 4, "all equal", pPassed);

    cout << "Partition tests: "
        << pPassed << "/8" << endl;
}

// Виведення статистики
void showStats(const char* name, const Stats& s) {
    cout << name
        << ": comparisons=" << s.comparisons
        << ", swaps=" << s.swaps
        << ", calls=" << s.calls
        << ", maxDepth=" << s.maxDepth << endl;
}

// Демонстрація трасування
void traceDemo() {
    cout << "\n=== TRACE DEMO ===" << endl;

    int source[] = { 9, 3, 7, 1, 8, 2, 6, 4 };
    int a[8];

    copyArray(source, a, 8);

    Stats l;

    cout << "-- Lomuto --" << endl;
    quickSortLomuto(a, 0, 7, l, 1, true);

    cout << "Sorted: ";
    printArray(a, 8);
    showStats("Lomuto", l);

    copyArray(source, a, 8);

    Stats h;

    cout << "-- Hoare --" << endl;
    quickSortHoare(a, 0, 7, h, 1, true);

    cout << "Sorted: ";
    printArray(a, 8);
    showStats("Hoare", h);
}

// Індивідуальний варіант 18
void variant18() {
    cout << "\n=== VARIANT 18 ===" << endl;

    int source[] = {
        37, 13, 24, 41, 9, 30, 17, 34,
        11, 21, 38, 8, 28, 15, 23
    };

    const int n = 15;
    int a[15];

    Stats l, h;

    cout << "Initial: ";
    printArray(source, n);

    copyArray(source, a, n);
    quickSortLomuto(a, 0, n - 1, l);

    cout << "Lomuto sorted: ";
    printArray(a, n);
    showStats("Lomuto", l);

    copyArray(source, a, n);
    quickSortHoare(a, 0, n - 1, h);

    cout << "Hoare sorted: ";
    printArray(a, n);
    showStats("Hoare", h);
}

// Створення наборів для експерименту
void fillData(int a[], int n, int type) {
    for (int i = 0; i < n; i++) {
        if (type == 0) {
            a[i] = i;
        }
        else if (type == 1) {
            a[i] = n - i;
        }
        else if (type == 2) {
            a[i] = (i * 37LL) % n;
        }
        else {
            a[i] = i % 5;
        }
    }
}

// Порівняльний експеримент
void experiment() {
    cout << "\n=== EXPERIMENT ===" << endl;

    const int sizes[] = { 100, 1000, 10000 };

    const char* types[] = {
        "ascending", "descending",
        "mixed", "duplicates"
    };

    cout << "n,type,algorithm,comparisons,swaps,calls,maxDepth"
        << endl;

    for (int si = 0; si < 3; si++) {
        int n = sizes[si];

        int source[MAX_N];
        int a[MAX_N];

        for (int type = 0; type < 4; type++) {
            fillData(source, n, type);

            copyArray(source, a, n);
            Stats l;

            quickSortLomuto(a, 0, n - 1, l);

            cout << n << "," << types[type] << ",Lomuto,"
                << l.comparisons << "," << l.swaps << ","
                << l.calls << "," << l.maxDepth << endl;

            copyArray(source, a, n);
            Stats h;

            quickSortHoare(a, 0, n - 1, h);

            cout << n << "," << types[type] << ",Hoare,"
                << h.comparisons << "," << h.swaps << ","
                << h.calls << "," << h.maxDepth << endl;
        }
    }
}

int main() {
    tests();
    traceDemo();
    variant18();
    experiment();

    return 0;
}
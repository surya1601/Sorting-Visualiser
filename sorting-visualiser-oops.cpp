#include <SDL2/SDL.h>
#include <cstdlib> 
#include <iostream> 
#include <ctime> 
#include <vector>

using namespace std;

const int height = 900;
const int width = 600;
const int mx_ele = 100;

class Visualizer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    vector<int> data;

public:
    Visualizer() : window(NULL), renderer(NULL), data(mx_ele) {}

    void init() {
        SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 6, 1);
        srand(time(0));
        for (int i = 0; i < mx_ele; i++) {
            data[i] = rand() % (height - 1) + 1;
        }
    }

    void destroy() {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }

    void draw_state(int red = -1, int blue = -1) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < mx_ele; i++) {
            if (i == red) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            } else if (i == blue) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_RenderDrawLine(renderer, i, height, i, height - data[i]);
        }
        SDL_RenderPresent(renderer);
    }

    void draw_end() {
        for (int i = 0; i < mx_ele; i++) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            for (int j = 0; j < i; j++) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, j, height, j, height - data[j]);
            }
            for (int j = i; j < min(i + 20, mx_ele); j++) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderDrawLine(renderer, j, height, j, height - data[j]);
            }
            for (int j = i + 20; j < mx_ele; j++) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, j, height, j, height - data[j]);
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(15);
        }
    }

    vector<int>& get_data() {
        return data;
    }
};

class SortingAlgorithm {
protected:
    Visualizer& visualizer;
    vector<int>& data;

public:
    SortingAlgorithm(Visualizer& visualizer) : visualizer(visualizer), data(visualizer.get_data()) {}

    virtual void sort() = 0;
};

class BubbleSort : public SortingAlgorithm {
public:
    BubbleSort(Visualizer& visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        for (int i = 0; i < data.size(); i++) {
            for (int j = i + 1; j < data.size(); j++) {
                if (data[j] < data[i]) swap(data[i], data[j]);
                visualizer.draw_state();
                SDL_Delay(1);
            }
        }
        visualizer.draw_end();
    }
};

class SelectionSort : public SortingAlgorithm {
public:
    SelectionSort(Visualizer& visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        for (int i = 0; i < mx_ele; i++) {
            int min_index = i;
            for (int j = i + 1; j < mx_ele; j++) {
                if (data[j] < data[min_index]) min_index = j;
            }
            swap(data[min_index], data[i]);
            visualizer.draw_state();
            SDL_Delay(50);
        }
        visualizer.draw_end();
    }
};

class InsertionSort : public SortingAlgorithm {
public:
    InsertionSort(Visualizer& visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        for (int i = 1; i < mx_ele; i++) {
            int val = data[i];
            int j = i - 1;
            while (val < data[j] && j >= 0) {
                data[j + 1] = data[j];
                j--;
                visualizer.draw_state();
                SDL_Delay(20);
            }
            data[j + 1] = val;
            visualizer.draw_state();
            SDL_Delay(20);
        }
        visualizer.draw_end();
    }
};

class MergeSort : public SortingAlgorithm {
public:
    MergeSort(Visualizer& visualizer) : SortingAlgorithm(visualizer) {}

    void merge(int p, int q, int r) {
        int n1 = q - p + 1;
        int n2 = r - q;
        vector<int> L(n1), M(n2);
        for (int i = 0; i < n1; i++) L[i] = data[p + i];
        for (int j = 0; j < n2; j++) M[j] = data[q + 1 + j];
        int i = 0, j = 0, k = p;
        while (i < n1 && j < n2) {
            if (L[i] <= M[j]) {
                data[k] = L[i];
                i++;
            } else {
                data[k] = M[j];
                j++;
            }
            k++;
            visualizer.draw_state();
            SDL_Delay(30);
        }
        while (i < n1) {
            data[k] = L[i];
            i++;
            k++;
            visualizer.draw_state();
            SDL_Delay(30);
        }
        while (j < n2) {
            data[k] = M[j];
            j++;
            k++;
            visualizer.draw_state();
            SDL_Delay(30);
        }
    }

    void mergeSort(int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(l, m);
            mergeSort(m + 1, r);
            merge(l, m, r);
            if (l == 0 && r == mx_ele - 1) visualizer.draw_end();
        }
    }

    void sort() override {
        mergeSort(0, mx_ele - 1);
    }
};

class QuickSort : public SortingAlgorithm {
public:
    QuickSort(Visualizer& visualizer) : SortingAlgorithm(visualizer) {}

    int partition(int low, int high) {
        int pivot = data[high];
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (data[j] <= pivot) {
                i++;
                swap(data[i], data[j]);
            }
        }
        swap(data[i + 1], data[high]);
        visualizer.draw_state();
        SDL_Delay(150);
        return (i + 1);
    }

    void quickSort(int low, int high) {
        if (low < high) {
            int pi = partition(low, high);
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
            if (low == 0 && high == mx_ele - 1) visualizer.draw_end();
        }
    }

    void sort() override {
        quickSort(0, mx_ele - 1);
    }
};

class SortingVisualizerApp {
private:
    Visualizer visualizer;

public:
    void run() {
        cout << "Welcome to the Sorting Visualiser - \n";
        cout << "You can visualise the following sorting types: \n";
        cout << "Bubble Sort (Press 1 and ENTER to run Bubble Sort)\n";
        cout << "Selection Sort (Press 2 and ENTER to run Selection Sort)\n";
        cout << "Insertion Sort (Press 3 and ENTER to run Insertion Sort)\n";
        cout << "Merge Sort (Press 4 and ENTER to run Merge Sort)\n";
        cout << "Quick Sort (Press 5 and ENTER to run Quick Sort)\n";
        cout << "Press -1 and ENTER to exit\n";

        int a;
        do {
            cout << "Enter your option: ";
            cin >> a;

            if (a == -1) break;

            visualizer.init();
            SortingAlgorithm* algorithm = nullptr;

            switch (a) {
                case 1:
                    algorithm = new BubbleSort(visualizer);
                    break;
                case 2:
                    algorithm = new SelectionSort(visualizer);
                    break;
                case 3:
                    algorithm = new InsertionSort(visualizer);
                    break;
                case 4:
                    algorithm = new MergeSort(visualizer);
                    break;
                case 5:
                    algorithm = new QuickSort(visualizer);
                    break;
                default:
                    cout << "There is no such option\n";
                    break;
            }

            if (algorithm) {
                algorithm->sort();
                delete algorithm;
            }

            visualizer.destroy();
            cout << "Press -1 and ENTER to exit\n";
            cout << "If you want to visualise any other Algorithm, press their corresponding number: ";
        } while (a != -1);

        cout << "Thank you. Hoping that you've enjoyed the visualisations\n";
    }
};

int main() {
    SortingVisualizerApp app;
    app.run();
    return 0;
}

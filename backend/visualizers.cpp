#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

// Persistent C++ backend. One tab-separated request is read per line;
// one JSON response is written per line. Data-structure state lives here.
string decode(const string &s)
{
    string out;
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (s[i] == '%' && i + 2 < s.size())
        {
            auto hex = [](char c) -> int
            { if(c>='0'&&c<='9') return c-'0'; if(c>='A'&&c<='F') return c-'A'+10; if(c>='a'&&c<='f') return c-'a'+10; return -1; };
            int a = hex(s[i + 1]), b = hex(s[i + 2]);
            if (a >= 0 && b >= 0)
            {
                out.push_back(char(a * 16 + b));
                i += 2;
            }
            else
                out.push_back(s[i]);
        }
        else if (s[i] == '+')
            out.push_back(' ');
        else
            out.push_back(s[i]);
    }
    return out;
}
string esc(const string &s)
{
    string o;
    for (char c : s)
    {
        if (c == '"' || c == '\\')
        {
            o += '\\';
            o += c;
        }
        else if (c == '\n')
            o += "\\n";
        else if (c == '\r')
            o += "\\r";
        else if (c == '\t')
            o += "\\t";
        else if ((unsigned char)c >= 32)
            o += c;
    }
    return o;
}
string arrJson(const vector<string> &a)
{
    string o = "[";
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (i)
            o += ",";
        o += "\"" + esc(a[i]) + "\"";
    }
    return o + "]";
}

class StackDS
{
    vector<string> a;
    size_t cap = 8;

public:
    string run(const string &op, const string &v = "")
    {
        string msg;
        bool ok = true;
        if (op == "push")
        {
            if (a.size() >= cap)
            {
                ok = false;
                msg = "Stack Overflow! Capacity is " + to_string(cap);
            }
            else
            {
                a.push_back(v);
                msg = "Pushed '" + v + "' to stack";
            }
        }
        else if (op == "pop")
        {
            if (a.empty())
            {
                ok = false;
                msg = "Stack Underflow! Nothing to pop";
            }
            else
            {
                string x = a.back();
                a.pop_back();
                msg = "Popped '" + x + "' from stack";
            }
        }
        else if (op == "peek")
        {
            if (a.empty())
            {
                ok = false;
                msg = "Stack is empty, nothing to peek";
            }
            else
                msg = "Top element is '" + a.back() + "' at index " + to_string(a.size() - 1);
        }
        else if (op == "clear")
        {
            a.clear();
            msg = "Stack cleared";
        }
        else
            msg = "Stack state loaded";
        return response(ok, msg);
    }
    string response(bool ok, const string &msg)
    {
        string top = a.empty() ? "Empty" : a.back();
        return string("{\"ok\":") + (ok ? "true" : "false") + ",\"message\":\"" + esc(msg) + "\",\"items\":" + arrJson(a) + ",\"size\":" + to_string(a.size()) + ",\"capacity\":" + to_string(cap) + ",\"top\":\"" + esc(top) + "\",\"isEmpty\":" + (a.empty() ? "true" : "false") + ",\"isFull\":" + (a.size() >= cap ? "true" : "false") + "}";
    }
};
class QueueDS
{
    static const int cap = 8;
    vector<string> a = vector<string>(cap);
    int frontIdx = 0, count = 0;
    vector<string> ordered() const
    {
        vector<string> out;
        for (int i = 0; i < count; i++)
            out.push_back(a[(frontIdx + i) % cap]);
        return out;
    }

public:
    string run(const string &op, const string &v = "")
    {
        bool ok = true;
        string msg;
        if (op == "enqueue")
        {
            if (count == cap)
            {
                ok = false;
                msg = "Queue Overflow! Capacity is " + to_string(cap);
            }
            else
            {
                int rear = (frontIdx + count) % cap;
                a[rear] = v;
                ++count;
                msg = "Enqueued '" + v + "' at rear";
            }
        }
        else if (op == "dequeue")
        {
            if (count == 0)
            {
                ok = false;
                msg = "Queue Underflow! Nothing to dequeue";
            }
            else
            {
                string x = a[frontIdx];
                a[frontIdx].clear();
                frontIdx = (frontIdx + 1) % cap;
                --count;
                msg = "Dequeued '" + x + "' from front";
            }
        }
        else if (op == "front")
        {
            if (count == 0)
            {
                ok = false;
                msg = "Queue is empty, no front element";
            }
            else
                msg = "Front element is '" + a[frontIdx] + "'";
        }
        else if (op == "clear")
        {
            for (auto &x : a)
                x.clear();
            frontIdx = 0;
            count = 0;
            msg = "Queue cleared";
        }
        else
            msg = "Queue state loaded";
        vector<string> items = ordered();
        string f = count == 0 ? "Empty" : a[frontIdx];
        return string("{\"ok\":") + (ok ? "true" : "false") + ",\"message\":\"" + esc(msg) + "\",\"items\":" + arrJson(items) + ",\"size\":" + to_string(count) + ",\"capacity\":" + to_string(cap) + ",\"front\":\"" + esc(f) + "\",\"isEmpty\":" + (count == 0 ? "true" : "false") + ",\"isFull\":" + (count == cap ? "true" : "false") + "}";
    }
};
struct Node
{
    string data;
    Node *next;
    Node(const string &v) : data(v), next(nullptr) {}
};
class LinkedListDS
{
    Node *head = nullptr;
    int n = 0;
    vector<string> values() const
    {
        vector<string> a;
        for (Node *t = head; t; t = t->next)
            a.push_back(t->data);
        return a;
    }
    void clear()
    {
        while (head)
        {
            Node *t = head;
            head = head->next;
            delete t;
        }
        n = 0;
    }

public:
    ~LinkedListDS() { clear(); }
    string run(const string &op, const string &v = "", int pos = 0)
    {
        bool ok = true;
        string msg;
        if (op == "insertBeginning")
        {
            Node *x = new Node(v);
            x->next = head;
            head = x;
            ++n;
            msg = "Inserted '" + v + "' at beginning (index 0)";
        }
        else if (op == "insertEnd")
        {
            Node *x = new Node(v);
            if (!head)
                head = x;
            else
            {
                Node *t = head;
                while (t->next)
                    t = t->next;
                t->next = x;
            }
            ++n;
            msg = "Inserted '" + v + "' at end (index " + to_string(n - 1) + ")";
        }
        else if (op == "insertAt")
        {
            if (pos < 0 || pos > n)
            {
                ok = false;
                msg = "Invalid position. Must be 0 to " + to_string(n);
            }
            else if (pos == 0)
            {
                Node *x = new Node(v);
                x->next = head;
                head = x;
                ++n;
                msg = "Inserted '" + v + "' at position 0";
            }
            else
            {
                Node *t = head;
                for (int i = 0; i < pos - 1; i++)
                    t = t->next;
                Node *x = new Node(v);
                x->next = t->next;
                t->next = x;
                ++n;
                msg = "Inserted '" + v + "' at position " + to_string(pos);
            }
        }
        else if (op == "deleteBeginning")
        {
            if (!head)
            {
                ok = false;
                msg = "List is empty";
            }
            else
            {
                Node *t = head;
                string x = t->data;
                head = head->next;
                delete t;
                --n;
                msg = "Deleted '" + x + "' from beginning";
            }
        }
        else if (op == "deleteEnd")
        {
            if (!head)
            {
                ok = false;
                msg = "List is empty";
            }
            else if (!head->next)
            {
                string x = head->data;
                delete head;
                head = nullptr;
                --n;
                msg = "Deleted '" + x + "' from end";
            }
            else
            {
                Node *t = head;
                while (t->next->next)
                    t = t->next;
                string x = t->next->data;
                delete t->next;
                t->next = nullptr;
                --n;
                msg = "Deleted '" + x + "' from end";
            }
        }
        else if (op == "deleteAt")
        {
            if (pos < 0 || pos >= n)
            {
                ok = false;
                msg = n == 0 ? "List is empty" : "Invalid position. Must be 0 to " + to_string(n - 1);
            }
            else if (pos == 0)
            {
                Node *t = head;
                string x = t->data;
                head = head->next;
                delete t;
                --n;
                msg = "Deleted '" + x + "' from position 0";
            }
            else
            {
                Node *t = head;
                for (int i = 0; i < pos - 1; i++)
                    t = t->next;
                Node *x = t->next;
                string val = x->data;
                t->next = x->next;
                delete x;
                --n;
                msg = "Deleted '" + val + "' from position " + to_string(pos);
            }
        }
        else if (op == "search")
        {
            Node *t = head;
            int i = 0, found = -1;
            while (t)
            {
                if (t->data == v)
                {
                    found = i;
                    break;
                }
                t = t->next;
                ++i;
            }
            if (found < 0)
            {
                ok = false;
                msg = "Value '" + v + "' not found in list";
            }
            else
                msg = "Found '" + v + "' at index " + to_string(found);
        }
        else if (op == "clear")
        {
            clear();
            msg = "List cleared (HEAD = NULL)";
        }
        else
            msg = "Linked List state loaded";
        vector<string> a = values();
        return string("{\"ok\":") + (ok ? "true" : "false") + ",\"message\":\"" + esc(msg) + "\",\"items\":" + arrJson(a) + ",\"size\":" + to_string(n) + ",\"head\":\"" + (a.empty() ? string("NULL") : esc(a[0])) + "\"}";
    }
};
class HashMapDS
{
    static const int N = 8;
    vector<pair<string, string>> b[N];
    int hashKey(const string &k)
    {
        unsigned int s = 0;
        for (unsigned char c : k)
            s += c;
        return s % N;
    }

public:
    string run(const string &op, const string &k = "", const string &v = "")
    {
        bool ok = true;
        string msg;
        int idx = hashKey(k);
        if (op == "insert")
        {
            bool updated = false;
            for (auto &e : b[idx])
                if (e.first == k)
                {
                    e.second = v;
                    updated = true;
                    break;
                }
            if (updated)
                msg = "Updated key '" + k + "' at bucket " + to_string(idx);
            else
            {
                bool collision = !b[idx].empty();
                b[idx].push_back({k, v});
                msg = "Inserted (" + k + "," + v + ") at bucket " + to_string(idx) + (collision ? " (Collision handled by chaining)" : "");
            }
        }
        else if (op == "search")
        {
            bool found = false;
            for (auto &e : b[idx])
                if (e.first == k)
                {
                    msg = "Found key '" + k + "' at bucket " + to_string(idx) + " with value: " + e.second;
                    found = true;
                    break;
                }
            if (!found)
            {
                ok = false;
                msg = "Key '" + k + "' not found (checked bucket " + to_string(idx) + ")";
            }
        }
        else if (op == "remove")
        {
            auto it = find_if(b[idx].begin(), b[idx].end(), [&](auto &e)
                              { return e.first == k; });
            if (it == b[idx].end())
            {
                ok = false;
                msg = "Key '" + k + "' not found, nothing deleted";
            }
            else
            {
                b[idx].erase(it);
                msg = "Deleted key '" + k + "' from bucket " + to_string(idx);
            }
        }
        else if (op == "clear")
        {
            for (auto &x : b)
                x.clear();
            msg = "HashMap cleared, all buckets empty";
        }
        else if (op == "loadExample")
        {
            for (auto &x : b)
                x.clear();
            vector<pair<string, string>> ex = {{"Alice", "25"}, {"Bob", "30"}, {"Charlie", "22"}, {"Bhavesh", "20"}};
            for (auto &e : ex)
                b[hashKey(e.first)].push_back(e);
            msg = "Loaded example: Alice, Bob, Charlie, Bhavesh";
        }
        else
            msg = "HashMap state loaded";
        int count = 0;
        string buckets = "[";
        for (int i = 0; i < N; i++)
        {
            if (i)
                buckets += ",";
            buckets += "[";
            for (size_t j = 0; j < b[i].size(); j++)
            {
                if (j)
                    buckets += ",";
                buckets += "{\"key\":\"" + esc(b[i][j].first) + "\",\"value\":\"" + esc(b[i][j].second) + "\"}";
                count++;
            }
            buckets += "]";
        }
        buckets += "]";
        return string("{\"ok\":") + (ok ? "true" : "false") + ",\"message\":\"" + esc(msg) + "\",\"buckets\":" + buckets + ",\"size\":" + to_string(count) + ",\"capacity\":8,\"bucket\":" + to_string(idx) + "}";
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    StackDS st;
    QueueDS qu;
    LinkedListDS ll;
    HashMapDS hm;
    string line;
    while (getline(cin, line))
    {
        vector<string> f;
        stringstream ss(line);
        string x;
        while (getline(ss, x, '\t'))
            f.push_back(decode(x));
        while (f.size() < 5)
            f.push_back("");
        string module = f[0], op = f[1], a = f[2], b = f[3];
        int pos = 0;
        try
        {
            pos = stoi(f[4]);
        }
        catch (...)
        {
        }
        string out;
        if (module == "stack")
            out = st.run(op, a);
        else if (module == "queue")
            out = qu.run(op, a);
        else if (module == "linkedlist")
            out = ll.run(op, a, pos);
        else if (module == "hashmap")
            out = hm.run(op, a, b);
        else
            out = "{\"ok\":false,\"message\":\"Unknown module\"}";
        cout << out << endl;
    }
    return 0;
}

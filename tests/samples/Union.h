union values {
    long l;
    int i;
    long long ll;
};

void setIntValue(union values *v);

void setLongValue(union values *v);

int getUnionSize();

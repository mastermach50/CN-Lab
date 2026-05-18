#include <stdio.h>

int main() {
    int bucket_size, inputnum, outrate;
    printf("Enter bucket size: ");
    scanf("%d", &bucket_size);
    printf("Enter outgoing rate: ");
    scanf("%d", &outrate);
    printf("Enter number of inputs: ");
    scanf("%d", &inputnum);

    int bucket = 0;
    for (int i = 0; i < inputnum; i++) {
        int insize;
        printf("Enter incoming packet size: ");
        scanf("%d", &insize);

        if (insize < bucket_size - bucket) {
            bucket += insize;
            printf("Packet is inserted into bucket\n");
            printf("\n");
        } else {
            printf("Packet is cannot fit in bucket\n");
            printf("Packet dropped\n");
            printf("\n");
        }

        printf("BUCKET: %d/%d\n", bucket, bucket_size);
        printf("\n");

        printf("%d bytes of packets were passed\n", outrate);
        printf("\n");

        bucket -= outrate;
        if (bucket < 0)
            bucket = 0;

        printf("BUCKET: %d/%d", bucket, bucket_size);
        printf("\n");
    }


    return 0;
}
#include "fromJson.h"
void executeJson(){
    FILE *fp;
    char buffer[1024];
    struct json_object *parsed_json;
    struct json_object *name;
    struct json_object *age;
    struct json_object *subjects;
    struct json_object *subject;
    size_t n_subjects;
    size_t i;
    fp = fopen("simpleTest.json","r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);
    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "name", &name);
    json_object_object_get_ex(parsed_json, "age", &age);
    json_object_object_get_ex(parsed_json, "subjects", &subjects);

    printf("Name: %s \n", json_object_get_string(name));
    printf("Age: %d \n", json_object_get_int(age));

    n_subjects = json_object_array_length(subjects);
    //printf("Found %lu subjects\n",n_subjects);

    for(i=0;i<n_subjects;i++) {
        subject = json_object_array_get_idx(subjects, i);
        printf("%lu. %s\n",i+1,json_object_get_string(subject));
    }
    /*Creating a json object*/
    json_object * jobj = json_object_new_object();

    /*Creating a json integer*/
    json_object *jint = json_object_new_int(10);


    /*Form the json object*/
    json_object_object_add(jobj,"Number of posts", jint);

    /*Now printing the json object*/
    printf ("The json object created: %sn",json_object_to_json_string(jobj));

}

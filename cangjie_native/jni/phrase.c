#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>

#define  LOG_TAG    "Cangjie"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include "phrase_internal.h"

int phrase_count = 0;
int phrase_index = 0;
int phrase_max   = 0;
int phrase_saved = 0;
int phrase_map[32768];
char phrase_path[1024];
int phrase_freq[sizeof(phrase) / sizeof(struct PHRASE_INDEX)];

void init_phrase()
{
  memset((char *) phrase_map, 0, sizeof(phrase_map));
  phrase_count = 0;
  phrase_index = 0;
  phrase_max   = 0;
  phrase_saved = 0;
}

int search_phrase(jchar index)
{
  int min = 0, max = sizeof(phraseindex) / sizeof(struct PHRASE_INDEX), mid = 0;
  int count = 0;
  int ch = (int) index;
  int loop = 1;
  int found = -1;
  int total = 0;
  /* LOGE("Phrase : %d", sizeof(phraseindex) / sizeof(struct PHRASE_INDEX)); */

  while (max > min) {
    if (phraseindex[min].c == index) {
      found = min;
      break;
    }
    if (phraseindex[max].c == index) {
      found = max;
      break;
    }
    if (min == max)
      break;
    mid = (min + max) / 2;
    if (phraseindex[mid].c == index) {
      found = mid;
      break;
    }
    if (phraseindex[mid].c > index) {
      max = mid - 1;
    } else {
      min = mid + 1;
    }
    total++;
  }
  if (found >= 0) {
    int count = 0;
    /* LOGE("Phrase : %d %d %d %d %d %d", total, found, phraseindex[found].index, phraseindex[found].size, ch, phraseindex[found].c); */
    phrase_index = phraseindex[found].index;
    phrase_count = phraseindex[found].size;
    phrase_max   = phraseindex[found].maxphrase;
    /* if (pcount >= 10) pcount = 10; */
    /* for (count = pindex; count < pindex + pcount; count++) { */
    /*   LOGE("Phrase : %s", phrase[count]); */
    /* } */
    for (count = 0; count < phrase_count; count++) {
      phrase_map[count] = phrase_index + count;
      /* LOGE("Phrase Map0 : %d %d %d", count, phrase_map[count], phrase_freq[phrase_map[count]]); */
    }

    int n = phrase_count;
    int swap = 1;
    int temp = 0;
    do {
      swap = 0;
      int i = 0;
      for (i = 0; i < n - 1; i++) {
    	if (phrase_freq[phrase_map[i]] < phrase_freq[phrase_map[i + 1]]) {
    	  temp = phrase_map[i];
    	  phrase_map[i] = phrase_map[i + 1];
    	  phrase_map[i + 1] = temp;
    	  swap = 1;
    	}
      }
    } while (swap);

    /* for (count = 0; count < phrase_count; count++) { */
    /*   LOGE("Phrase Map1 : %d %d %d", count, phrase_map[count], phrase_freq[phrase_map[count]]); */
    /* } */
    /* LOGE("Search Phrase 3"); */

  } else {
    phrase_index = 0;
    phrase_count = 0;
    phrase_max   = 0;
    memset(phrase_map, 0, sizeof(phrase_map));
    /* LOGE("No Phrase : %d", sizeof(phraseindex) / sizeof(struct PHRASE_INDEX)); */
  }

  return phrase_count;
}

int get_phrase_count()
{
  return phrase_count;
}

int get_phrase_index()
{
  return phrase_index;
}

int get_phrase_max()
{
  return phrase_max;
}

jchar* get_phrase(int index)
{
  /* LOGE("Get Phrase : %d %d %d", index, phrase_index, index - phrase_index); */
  return &phrase[phrase_map[index - phrase_index]][1];
}

int get_phrase_length(int index)
{
  return (int) phrase[index][0];
}

void update_phrase_frequency(int index)
{
  /* LOGE("Update Phrase Frequency : %d", index); */
  phrase_saved = 1;
  phrase_freq[phrase_map[index - phrase_index]]++;
}

void load_phrase(char *path)
{
  int clear = 1;
  char key[8];
  char buf[8];

  strncpy(phrase_path,          path, sizeof(phrase_path));
  strncat(phrase_path, "/phrase.dat", sizeof(phrase_path));

  memset(key, 0, 8);
  strcpy(key, "PHRAS0");

  FILE *file = fopen(path, "r");
  if (file != 0) {
    int read = fread(buf, 1, sizeof(buf), file);
    if (memcmp(buf, key, 8) == 0) {
      int read = fread(phrase_freq, 1, sizeof(phrase_freq), file);
      fclose(file);
      if (read == sizeof(phrase_freq)) clear = 0;
    }
  }

  if (clear != 0) {
    memset(phrase_freq, 0, sizeof(phrase_freq));
  }
}

void save_phrase()
{
  char key[8];

  if (phrase_saved == 0) return;
  phrase_saved = 1;

  memset(key, 0, 8);
  strcpy(key, "PHRAS0");
  FILE *file = fopen(phrase_path, "w");
  if (file != NULL) {
    fwrite(key, 1, sizeof(key), file);
    fwrite(phrase_freq, 1, sizeof(phrase_freq), file);
    fclose(file);
  }
}

void clear_phrase()
{
  memset(phrase_freq, 0, sizeof(phrase_freq));
}


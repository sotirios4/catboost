#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "c_api.h"

void LoggerError(const char *message, const char *error) {
	time_t now;
	char buffer[20];

	time(&now);
	strftime(buffer, 20, "%Y-%m-%dT%H:%M:%S", localtime(&now));

	fprintf(stderr, "%s \t ERROR \t CatBoost %s \t %s\n", buffer, message, error);
}

int main(int argc, char **argv) {

	if(argc != 2) {
		LoggerError("Error you didn't provide correctly model filename", "");
		return 1;
	}

	ModelCalcerHandle* modelHandle;
	modelHandle = ModelCalcerCreate();
	if (!LoadFullModelFromFile(modelHandle,	argv[1])) {
		LoggerError("LoadFullModelFromFile error message:", GetErrorString());
		return 1;
	}

	int floatFeaturesSize = GetFloatFeaturesCount(modelHandle);
	float floatFeatures[floatFeaturesSize];

	int resultSize = floatFeaturesSize - 1;
	double result[resultSize];

	char *line = NULL;
	size_t len = 0;
	char seps[] = ",";
	char *token;
	float var;
	ssize_t read = 0;
	int j = 0;

	while (read = getline(&line, &len, stdin) != -1) {
		token = strtok(line, seps);
		while (token != NULL) {
			sscanf(token, "%f", &var);
			floatFeatures[j++] = var;

			token = strtok(NULL, seps);
		}

		const float *features = floatFeatures;


		if (!CalcModelPredictionFlat(modelHandle, 1, &features, floatFeaturesSize, result, resultSize)) {
			LoggerError("CalcModelPredictionFlat error message:", GetErrorString());
			return 1;
		}

		for (int i = 0; i < resultSize; i++) {
			printf("%.6f ", result[i]);
		}
		j = 0;
		printf("\n");
	}

	free(line);
	ModelCalcerDelete(modelHandle);

	return 0;
}

all:
	gcc -o code/main code/main.c
	@echo "Compilação concluída com sucesso."
	@echo "Para executar, utilize: make run input.txt output.txt error.txt"
	@echo "O arquivo input.txt deve estar no mesmo diretório que o Makefile."

run:
	code/main $(INPUT) $(OUTPUT) $(ERROR)

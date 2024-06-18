all:
	gcc -o main main.c
	@echo "Compilação concluída com sucesso."
	@echo "Para executar, utilize: make run INPUT=input.txt OUTPUT=output.txt"
	@echo "O arquivo input.txt deve estar no mesmo diretório que main.c"

run:
	./main $(INPUT) $(OUTPUT)

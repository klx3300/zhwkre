zhwkre_bubble_sort.o: algorithm/bubble_sort.c
	@echo "   gcc   zhwkre_bubble_sort.o"
	@gcc -c  -o zhwkre_bubble_sort.o algorithm/bubble_sort.c
zhwkre_bss.o: bss/bss.c
	@echo "   gcc   zhwkre_bss.o"
	@gcc -c  -o zhwkre_bss.o bss/bss.c
zhwkre_mutex.o: concurrent/mutex.c
	@echo "   gcc   zhwkre_mutex.o"
	@gcc -c  -o zhwkre_mutex.o concurrent/mutex.c
zhwkre_threading.o: concurrent/threading.c
	@echo "   gcc   zhwkre_threading.o"
	@gcc -c  -o zhwkre_threading.o concurrent/threading.c
zhwkre_list.o: containers/list.c
	@echo "   gcc   zhwkre_list.o"
	@gcc -c  -o zhwkre_list.o containers/list.c
zhwkre_unordered_map.o: containers/unordered_map.c
	@echo "   gcc   zhwkre_unordered_map.o"
	@gcc -c  -o zhwkre_unordered_map.o containers/unordered_map.c
zhwkre_socket.o: network/socket.c
	@echo "   gcc   zhwkre_socket.o"
	@gcc -c  -o zhwkre_socket.o network/socket.c
zhwkre_tcp.o: network/tcp.c
	@echo "   gcc   zhwkre_tcp.o"
	@gcc -c  -o zhwkre_tcp.o network/tcp.c
zhwkre_udp.o: network/udp.c
	@echo "   gcc   zhwkre_udp.o"
	@gcc -c  -o zhwkre_udp.o network/udp.c
zhwkre_serialization.o: serialization/serialization.c
	@echo "   gcc   zhwkre_serialization.o"
	@gcc -c  -o zhwkre_serialization.o serialization/serialization.c
zhwkre_utils.o: utils/utils.c
	@echo "   gcc   zhwkre_utils.o"
	@gcc -c  -o zhwkre_utils.o utils/utils.c
zhwkre_error.o: error/error.c
	@echo "   gcc   zhwkre_error.o"
	@gcc -c  -o zhwkre_error.o error/error.c
zhwkre_btree.o: containers/btree.c
	@echo "   gcc   zhwkre_btree.o"
	@gcc -c -g -o zhwkre_btree.o containers/btree.c
zhwkre_btree_test.o: tests/btree_test.c
	@echo "   gcc   zhwkre_btree_test.o"
	@gcc -c -g -o zhwkre_btree_test.o tests/btree_test.c
btree_test: zhwkre_btree_test.o zhwkre_btree.o zhwkre_error.o 
	@echo "   LD   btree_test"
	@gcc  -o btree_test zhwkre_btree_test.o zhwkre_btree.o zhwkre_error.o 
clean:
	@echo "  CLEAN   all files."
	-@rm zhwkre_bubble_sort.o zhwkre_bss.o zhwkre_mutex.o zhwkre_threading.o zhwkre_list.o zhwkre_unordered_map.o zhwkre_socket.o zhwkre_tcp.o zhwkre_udp.o zhwkre_serialization.o zhwkre_utils.o zhwkre_error.o zhwkre_btree.o zhwkre_btree_test.o btree_test  || true

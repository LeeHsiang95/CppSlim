#pragma once
#include <string>
#include <memory>
#include <list>

	class SlimList;
	class StatementExecutor;

	class Node {
	public:
		Node(const std::string& str);
		~Node();

		std::string GetString();
		std::shared_ptr<SlimList> GetList();
		void Replace(const std::string& str);

		std::shared_ptr<Node> next_{nullptr};
		std::shared_ptr<SlimList> list_{nullptr};
		std::string str_data_;
	};

	class SlimList : public std::enable_shared_from_this<SlimList> {
	public:
		std::list<std::shared_ptr<Node>> nodes_;

		std::list<std::shared_ptr<Node>>::iterator GetBegin() { return nodes_.begin(); }
		std::shared_ptr<Node> GetHead() { if (nodes_.empty())return nullptr; return nodes_.front(); }
		std::shared_ptr<Node> GetTail() { if (nodes_.empty())return nullptr; return nodes_.back(); }

	public:
		SlimList() = default;
		SlimList(const std::string& raw_data);
		~SlimList();

		static bool IsValid(const std::string& raw_data);

		std::shared_ptr<Node> CreateIterator();
		void AddBuffer(const std::string& str);
		void AddString(const std::string& str);
		void AddList(std::shared_ptr<SlimList> element);
		void PopHead();
		int GetLength();
		bool Equals(std::shared_ptr<SlimList> other);
		std::shared_ptr<Node> GetNodeAt(int index);
		void insertNode(std::shared_ptr<Node> node);

		std::shared_ptr<SlimList> GetListAt(int index);
		std::string GetStringAt(int index);
		double GetDoubleAt(int index);
		std::shared_ptr<SlimList> GetHashAt(int index);
		void ReplaceAt(int index, char const* replacementString);
		std::shared_ptr<SlimList> GetTailAt(int index);
		std::string ToString(); /// Use CSlim_DestroyString to deallocate the string

		std::string Serialize();

		int SerializedLength();

		std::shared_ptr<Node> Advance(std::shared_ptr<Node> node);
		std::shared_ptr<Node> AdvanceBy(std::shared_ptr<Node> node, int);

		void AddResult(const std::string& id, const std::string& result);

		std::shared_ptr<SlimList> Execute(std::shared_ptr<StatementExecutor> executor);

	protected:
		static int readLength(char const** readPtr);
		static int ByteLength(int characterLength, char const* current);

		static std::string GetStringWithNullAsANormalString(std::shared_ptr<Node> iterator);
		static long FieldLength(const std::string& str);

		std::string Dispatch(std::shared_ptr<StatementExecutor> executor);
		std::string CallAndAssign(std::shared_ptr<StatementExecutor> executor);
		std::string Call(std::shared_ptr<StatementExecutor> executor);
		std::string Make(std::shared_ptr<StatementExecutor> executor);

		std::string SlimList::InvalidCommand();
		std::string SlimList::MalformedInstruction();
		std::string SlimList::Import();
	};

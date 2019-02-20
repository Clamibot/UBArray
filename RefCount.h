#ifndef	REF_COUNT_H
#define	REF_COUNT_H

class RefCount
	{
	private:
		int		References;
	public:
		RefCount() : References(0)
		{

		}
		~RefCount()
		{

		}
		void	UpCount		();
		void	DownCount	();
	};

inline void RefCount::UpCount ()
	{
	++References;
	}

inline void RefCount::DownCount ()
	{
	if (--References == 0)
			delete this;
		else;
	}

#endif

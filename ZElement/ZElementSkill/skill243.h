#ifndef __CPPGEN_GNET_SKILL243
#define __CPPGEN_GNET_SKILL243
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill243:public Skill
    {
      public:
        enum
        { SKILL_ID = 243 };
          Skill243 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill243Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill243Stub ():SkillStub (243)
        {
            occupation = 1;
            name = L"×··çÖä";
            nativename = "×··çÖä";
            icon = "×··çÖä.dds";
            maxlevel = 12;
            maxlearn = 5;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 619;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[1] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[2] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[3] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[4] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[5] = "¹íÍõ×Ú_¿ñ·çÖä";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[16] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[17] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[18] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[19] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[20] = "¹íÍõ×Ú_Æï³Ë_¿ñ·çÖä";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 5;
            doenchant = true;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill243Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 244 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.25 * skill->GetLevel () * skill->GetLevel () + 2.8 * skill->GetLevel () + 31);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 80000 - 10000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 16, 18, 19, 22, 25 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.25 * skill->GetLevel () * skill->GetLevel () + 2.8 * skill->GetLevel () + 31),
                               10 + 2 * skill->GetLevel (), 0.2 + 0.4 * skill->GetLevel (), skill->GetLevel () * 2);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000 + 2000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (0.2 + 0.4 * skill->GetLevel ());
            skill->GetVictim ()->SetAddspeed (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000 + (2000 + 1000 * skill->GetT0 ()) * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (2 * skill->GetLevel () * (1 + skill->GetT0 ()));
            skill->GetVictim ()->SetAdddodge (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () == 2 ? 120 : 0));
            skill->GetVictim ()->SetTime (11000 * skill->GetLevel () * skill->GetT0 () + 100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetValue (0.5);
            skill->GetVictim ()->SetIncmountspeed (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * norm (skill->GetT0 ())));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (2 * skill->GetT0 ());
            skill->GetVictim ()->SetIncmountspeed (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (8000 * int (skill->GetT0 () / 1.5));
              skill->GetVictim ()->SetImmuneslow (1);
              return true;
        }
#endif
    };
}
#endif
